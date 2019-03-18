#include "InGameModeBase.h"
#include "DefaultCharacter.h"
#include "WaitingToJoinWidget.h"
#include "GameFramework/PlayerController.h"
#include "HANSEIRacingGameInstance.h"
#include "HANSEIRacginPlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "engine/Engine.h"
#include <algorithm>

AInGameModeBase::AInGameModeBase() : m_bSpawnNewPlayer(false), m_MyCharacter(nullptr), m_PlayerController(nullptr), m_GameInstance(nullptr), m_bSpawned(false) {
	PlayerControllerClass = AHANSEIRacginPlayerController::StaticClass();

	m_Port = 3500;
	m_SocketName = "InGameSocket";
	PrimaryActorTick.bCanEverTick = true;
}

void AInGameModeBase::BeginPlay() {
	ABaseGameMode::BeginPlay();

	m_PlayerController = Cast<AHANSEIRacginPlayerController>(GetWorld()->GetFirstPlayerController());
	m_GameInstance = Cast<UHANSEIRacingGameInstance>(GetGameInstance());
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), m_SpawnPoints);

	if (GetClientSocket() && m_GameInstance) {
		m_GameInstance->SetUniqueKey(-1);
		SendMessageToServerAboutConnectState(m_GameInstance->GetSessionName(), m_GameInstance->GetPlayerNickName());
	}
}

void AInGameModeBase::Tick(float DeltaTime) {
	ABaseGameMode::Tick(DeltaTime);

	if (IsInGameThread()) {
		if (m_bSpawnNewPlayer) {
			SpawnCharacter();
		}

		UpdatePlayerLocationAndRotation();
	}
}

void AInGameModeBase::BeginDestroy() {
	SendDisconnect();
	ABaseGameMode::BeginDestroy();
}

void AInGameModeBase::RecvDataProcessing(TCHAR* RecvMessage) {
	std::stringstream RecvStream(TCHAR_TO_ANSI(RecvMessage));
	int32 PacketType = -1;
	RecvStream >> PacketType;

	switch (PacketType) {
	case PGM_NEWPLAYER:
		IsSucceedJoinOtherPlayer(RecvStream);
		break;
	case PGM_JOIN:
		IsSucceedJoinSession(RecvStream);
		break;
	case PGM_DISCONNECTOTHER:
		DisconnectOtherPlayer(RecvStream);
		break;
	case PGM_UPDATE:
		UpdateCharacterInformation(RecvStream);
		break;
	}
}

bool AInGameModeBase::SendMessageToServerAboutConnectState(const FString& SessionName, const FString& NickName) {
	int32 SendBytes = 0;
	if (GetClientSocket() && GetClientSocket()->GetConnectionState() == ESocketConnectionState::SCS_Connected) {
		std::stringstream SendStream;
		SendStream << PGM_JOIN << std::endl << TCHAR_TO_ANSI(*SessionName) << std::endl << TCHAR_TO_ANSI(*NickName) << std::endl;

		if (GetClientSocket()->Send(SendStream.str().c_str(), SendStream.str().length())) {
			return true;
		}
	}
	return false;
}

void AInGameModeBase::SendPlayerLocationAndRotation(const PLAYER::Vector& Location, const PLAYER::Vector& Rotation) {
	PLAYER::Character CharacterInformation;
	std::stringstream SendStream;

	if (m_GameInstance) {
		CharacterInformation.m_UniqueKey = m_GameInstance->GetUniqueKey();
		CharacterInformation.m_PlayerName = TCHAR_TO_ANSI(*m_GameInstance->GetPlayerNickName());
		CharacterInformation.m_Location = Location;
		CharacterInformation.m_Rotation = Rotation;

		SendStream << PGM_UPDATE << std::endl << TCHAR_TO_ANSI(*m_GameInstance->GetSessionName()) << std::endl << CharacterInformation;
		if (GetClientSocket()) {
			GetClientSocket()->Send(SendStream.str().c_str(), SendStream.str().length());
		}
	}
}

void AInGameModeBase::SpawnCharacter() {
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (int32 i = m_Players.Num(); i < m_PlayersInformation.m_Characters.size(); i++) {
		ADefaultCharacter* NewPawn = GetWorld()->SpawnActor<ADefaultCharacter>(ADefaultCharacter::StaticClass(), m_SpawnPoints[i]->GetActorLocation(), FRotator(0.f), Params);

		if (m_GameInstance->GetUniqueKey() == m_PlayersInformation.m_Characters[i].m_UniqueKey && !m_MyCharacter && m_PlayerController && m_PlayerController->GetPawn()) {
			FInputModeGameOnly InputMode;
			m_PlayerController->GetPawn()->Destroy();
			m_PlayerController->Possess(NewPawn);
			m_PlayerController->SetInputMode(InputMode);
			m_PlayerController->LocationSendTimerStart(this);
			m_PlayerController->bShowMouseCursor = false;
			m_MyCharacter = NewPawn;
			m_bSpawned = true;
		}
		else {
			NewPawn->SpawnDefaultController();
		}
		m_Players.Add(m_PlayersInformation.m_Characters[i].m_UniqueKey, NewPawn);
	}
	m_bSpawnNewPlayer = false;
}

void AInGameModeBase::UpdatePlayerLocationAndRotation() {
	for (int i = 0; i < m_PlayersInformation.m_Characters.size(); i++) {
		std::vector<PLAYER::Character>& CharactersInformation = m_PlayersInformation.m_Characters;
		if (i < m_Players.Num() && m_GameInstance->GetUniqueKey() != CharactersInformation[i].m_UniqueKey) {
			auto Character = m_Players.Find(CharactersInformation[i].m_UniqueKey);
			if (Character) {
				FVector Location(CharactersInformation[i].m_Location.X, CharactersInformation[i].m_Location.Y, CharactersInformation[i].m_Location.Z);
				FRotator Rotation(CharactersInformation[i].m_Rotation.X, CharactersInformation[i].m_Rotation.Y, CharactersInformation[i].m_Rotation.Z);
				(*Character)->SetActorLocationAndRotation(Location, FQuat(Rotation));
			}
		}
	}
}

void AInGameModeBase::IsSucceedJoinSession(std::stringstream& RecvStream) {
	uint32 UniqueKey = 0;
	int32 IsSucceed = -1;
	RecvStream >> IsSucceed >> UniqueKey;

	if (IsSucceed == GAMEPACKET::EJS_SUCCEDD && m_GameInstance) {
		m_GameInstance->SetUniqueKey(UniqueKey);
		RecvStream >> m_PlayersInformation;
		m_bSpawnNewPlayer = true;
	}
}

void AInGameModeBase::IsSucceedJoinOtherPlayer(std::stringstream& RecvStream) {
	int32 IsSucceed = -1;
	RecvStream >> IsSucceed;

	if (IsSucceed == GAMEPACKET::EJS_SUCCEDD) {
		RecvStream >> m_PlayersInformation;
		m_bSpawnNewPlayer = true;
	}
}

void AInGameModeBase::SendDisconnect() {
	std::stringstream SendStream;
	
	if (m_GameInstance) {
		SendStream << PGM_DISCONNECT << std::endl << TCHAR_TO_ANSI(*m_GameInstance->GetSessionName()) << std::endl << m_GameInstance->GetUniqueKey() << std::endl;
		if (GetClientSocket()) {
			GetClientSocket()->Send(SendStream.str().c_str(), SendStream.str().length());
		}
	}
}

void AInGameModeBase::DisconnectOtherPlayer(std::stringstream& RecvStream) {
	uint32 UniqueKey = 0;
	RecvStream >> UniqueKey;
	
	std::unique_lock<std::mutex> Lock(m_Lock);
	
	auto CharacterInformation = std::find_if(m_PlayersInformation.m_Characters.begin(), m_PlayersInformation.m_Characters.end(), [&UniqueKey](const PLAYER::Character& Character) -> bool { if (Character.m_UniqueKey == UniqueKey) { return true; } return false; });
	ADefaultCharacter* Character = nullptr;
	if (CharacterInformation != m_PlayersInformation.m_Characters.cend()) {
		m_PlayersInformation.m_Characters.erase(CharacterInformation);
		m_Players.RemoveAndCopyValue(UniqueKey, Character);
		if (Character) {
			Character->SetIsDisconnect(true);
		}
	}
}

void AInGameModeBase::UpdateCharacterInformation(std::stringstream& RecvStream) {
	uint32 UniqueKey = 0;
	PLAYER::Character Character;

	RecvStream >> UniqueKey >> Character;

	auto CharacterInformation = std::find_if(m_PlayersInformation.m_Characters.begin(), m_PlayersInformation.m_Characters.end(), [&UniqueKey](const PLAYER::Character& Character) -> bool { if (Character.m_UniqueKey == UniqueKey) { return true; } return false; });
	if (CharacterInformation != m_PlayersInformation.m_Characters.cend()) {
		(*CharacterInformation) = Character;
	}
}