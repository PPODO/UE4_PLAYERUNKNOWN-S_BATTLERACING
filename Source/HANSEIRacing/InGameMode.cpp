#include "InGameMode.h"
#include "InGameWidget.h"
#include "HANSEIRacingController.h"
#include "HANSEIRacingGameInstance.h"
#include "DefaultVehicleCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "StartPoint.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include <memory>
#include <algorithm>

AInGameMode::AInGameMode() : m_GameInstance(nullptr), m_Character(nullptr), m_bIsSpawnPlayer(false), m_SocketNumber(0), m_bIsInGame(false), m_bIsTeleport(false) {
	PlayerControllerClass = AHANSEIRacingController::StaticClass();

	m_Port = 3500;
	m_SocketName = L"Login_Socket";

	PrimaryActorTick.bCanEverTick = true;
}

void AInGameMode::BeginPlay() {
	ABaseGameMode::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), m_SpawnPoint);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStartPoint::StaticClass(), m_StartPoint);

	m_GameInstance = Cast<UHANSEIRacingGameInstance>(GetGameInstance());
	if(!m_GameInstance){
		m_GameInstance = Cast<UHANSEIRacingGameInstance>(GetGameInstance());
	}
	SendJoinGameToServer();
}

void AInGameMode::Tick(float DeltaTime) {
	ABaseGameMode::Tick(DeltaTime);

	if (IsInGameThread()) {
		if (m_bIsSpawnPlayer) {
			SpawnCharacter();
		}
		if (m_bIsTeleport) {
			TeleportCharacters();
		}

		UpdatePlayerLocationAndRotation();
	}
}

void AInGameMode::BeginDestroy() {
	SendDisconnectToServer();

	ABaseGameMode::BeginDestroy();
}

void AInGameMode::RecvDataProcessing(uint8* RecvBuffer, int32& RecvBytes) {
	PACKET* Packet = (PACKET*)RecvBuffer;
	if (Packet) {
		int32 PacketSize = CalculatePacketSize(Packet);
		m_PacketStack.push(Packet);

		for (int32 i = 0; i < (RecvBytes / PacketSize) - 1; i++) {
			uint8* ShiftBuffer = RecvBufferShiftProcess(RecvBuffer, RecvBytes, PacketSize);
			if (ShiftBuffer) {
				m_PacketStack.push((PACKET*)ShiftBuffer);
				PacketSize = CalculatePacketSize(m_PacketStack.top());
			}
		}
	}
	
	while (!m_PacketStack.empty()) {
		PACKET* Packet = m_PacketStack.top();
		if (Packet->m_MessageType == EPACKETMESSAGEFORGAMETYPE::EPMGT_JOIN) {
			IsSucceedJoinGame(*static_cast<GAMEPACKET*>(Packet));
		}
		else if (Packet->m_MessageType == EPACKETMESSAGEFORGAMETYPE::EPMGT_NEWPLAYER) {
			IsSucceedJoinGameNewPlayer(*static_cast<GAMEPACKET*>(Packet));
		}
		else if (Packet->m_MessageType == EPACKETMESSAGEFORGAMETYPE::EPMGT_DISCONNECTOTHER) {
			IsSucceedDisconnectOtherPlayer(*static_cast<GAMEPACKET*>(Packet));
		}
		else if (Packet->m_MessageType == EPACKETMESSAGEFORGAMETYPE::EPMGT_UPDATE) {
			IsSucceedUpdatePlayerInformation(*static_cast<GAMEPACKET*>(Packet));
		}
		else if (Packet->m_MessageType == EPACKETMESSAGEFORGAMETYPE::EPMGT_STARTGAME) {
			IsSucceedStartGame(*static_cast<GAMEPACKET*>(Packet));
		}
		else if (Packet->m_MessageType == EPACKETMESSAGEFORGAMETYPE::EPMGT_SPAWNITEM) {

		}
		m_PacketStack.pop();
	}
}

// FROM Server

void AInGameMode::IsSucceedJoinGame(GAMEPACKET& Packet) {
	if (Packet.m_FailedReason == EPACKETFAILEDTYPE::EPFT_SUCCEED) {
		if (m_GameInstance && m_GameInstance->IsValidLowLevel()) {
			m_GameInstance->SetUniqueKey(Packet.m_UniqueKey);
		}
		m_PlayerList.push_back(Packet);
		m_bIsSpawnPlayer = true;
		m_SocketNumber = Packet.m_Socket;
	}
}

void AInGameMode::IsSucceedJoinGameNewPlayer(GAMEPACKET& Packet) {
	if (Packet.m_FailedReason == EPACKETFAILEDTYPE::EPFT_SUCCEED) {
		m_PlayerList.push_back(Packet);
		m_bIsSpawnPlayer = true;
	}
}

void AInGameMode::IsSucceedDisconnectOtherPlayer(GAMEPACKET& Packet) {
	if (Packet.m_FailedReason == EPACKETFAILEDTYPE::EPFT_SUCCEED) {
		auto Character = m_CharacterClass.Find(Packet.m_UniqueKey);
		auto CharacterList = std::find_if(m_PlayerList.begin(), m_PlayerList.end(), [&](const GAMEPACKET& Data) -> bool { if (Data.m_UniqueKey == Packet.m_UniqueKey) { return true; } return false; });
		if (Character != nullptr && CharacterList != m_PlayerList.cend()) {
			ResetPlayerCurrentRank(Packet.m_UniqueKey, (*Character)->GetPlayerCurrentRank());
			(*Character)->SetIsDisconnect(true);
			m_PlayerList.erase(CharacterList);
			m_CharacterClass.Remove(Packet.m_UniqueKey);
		}
		if (IsValid(m_InGameWidget)) {
			m_InGameWidget->SetCharacterClassData(m_CharacterClass);
		}
	}
}

void AInGameMode::IsSucceedUpdatePlayerInformation(GAMEPACKET& Packet) {
	if (Packet.m_FailedReason == EPACKETFAILEDTYPE::EPFT_SUCCEED) {
		auto Character = std::find_if(m_PlayerList.begin(), m_PlayerList.end(), [&](const GAMEPACKET& Data) -> bool { if (Data.m_UniqueKey == Packet.m_UniqueKey) { return true; } return false; });
		if (Character != m_PlayerList.cend()) {
			(*Character) = Packet;
		}
	}
}

void AInGameMode::IsSucceedStartGame(GAMEPACKET& Packet) {
	if (Packet.m_FailedReason == EPACKETFAILEDTYPE::EPFT_SUCCEED) {
		m_bIsTeleport = m_bIsInGame = true;
	}
}

// TO Server

void AInGameMode::SendJoinGameToServer() {
	GAMEPACKET Packet;

	if (GetSocket() && m_GameInstance && m_GameInstance->IsValidLowLevel()) {
		Packet.m_PacketType = EPACKETTYPE::EPT_PLAYER;
		Packet.m_MessageType = EPACKETMESSAGEFORGAMETYPE::EPMGT_JOIN;
		Packet.m_SessionID = m_GameInstance->GetSessionID();
		Packet.m_Socket = m_SocketNumber;
		strncpy(Packet.m_PlayerNickName, TCHAR_TO_ANSI(*m_GameInstance->GetPlayerNickName()), MaxNameLen);

		GetSocket()->Send((char*)&Packet, sizeof(GAMEPACKET));
	}
}

void AInGameMode::SendCharacterInformationToServer(const FVector& Location, const FRotator& Rotation, const FInputMotionData& Data) {
	GAMEPACKET Packet;

	if (GetSocket() && m_Character && m_GameInstance && m_GameInstance->IsValidLowLevel()) {
		Packet.m_PacketType = EPACKETTYPE::EPT_PLAYER;
		Packet.m_MessageType = EPACKETMESSAGEFORGAMETYPE::EPMGT_UPDATE;
		Packet.m_SessionID = m_GameInstance->GetSessionID();
		Packet.m_UniqueKey = m_GameInstance->GetUniqueKey();
		Packet.m_Socket = m_SocketNumber;
		Packet.m_Location = Location;
		Packet.m_Rotation = Rotation;
		Packet.m_VehicleData = Data;

		GetSocket()->Send((char*)&Packet, sizeof(GAMEPACKET));
	}
}

void AInGameMode::SendDisconnectToServer() {
	GAMEPACKET Packet;

	if (GetSocket() && m_GameInstance && m_GameInstance->IsValidLowLevel()) {
		Packet.m_PacketType = EPACKETTYPE::EPT_PLAYER;
		Packet.m_MessageType = EPACKETMESSAGEFORGAMETYPE::EPMGT_DISCONNECT;
		Packet.m_SessionID = m_GameInstance->GetSessionID();
		Packet.m_UniqueKey = m_GameInstance->GetUniqueKey();
		Packet.m_Socket = m_SocketNumber;

		GetSocket()->Send((char*)&Packet, sizeof(GAMEPACKET));
	}
}

void AInGameMode::SendStartGame() {
	if (!m_bIsInGame) {
		GAMEPACKET Packet;

		if (GetSocket()) {
			Packet.m_PacketType = EPACKETTYPE::EPT_PLAYER;
			Packet.m_MessageType = EPACKETMESSAGEFORGAMETYPE::EPMGT_STARTGAME;
			Packet.m_SessionID = m_GameInstance->GetSessionID();
			Packet.m_Socket = m_SocketNumber;

			GetSocket()->Send((ANSICHAR*)&Packet, sizeof(GAMEPACKET));
		}
	}
}

// Private Function

int32 AInGameMode::CalculatePacketSize(const PACKET* Packet) {
	switch (Packet->m_PacketType) {
	case EPACKETTYPE::EPT_PLAYER:
		return sizeof(GAMEPACKET);
	case EPACKETTYPE::EPT_SPAWNER:
		return sizeof(1);
	}
	return 0;
}

AActor* AInGameMode::FindSpawnPointByUniqueKey(const int32& UniqueKey) {
	AActor* SpawnPoint = nullptr;
	for (auto It : m_SpawnPoint) {
		if (Cast<APlayerStart>(It) && Cast<APlayerStart>(It)->PlayerStartTag.Compare(*FString::Printf(L"%d", UniqueKey)) == 0) {
			SpawnPoint = It;
			break;
		}
	}
	return SpawnPoint;
}

void AInGameMode::SpawnPawnAndAddCharacterList(ADefaultVehicleCharacter* NewPawn, const int32& UniqueKey, const ANSICHAR* PlayerName) {
	if (!m_Character && UniqueKey == m_GameInstance->GetUniqueKey()) {
		if (Cast<AHANSEIRacingController>(GetWorld()->GetFirstPlayerController()) && GetWorld()->GetFirstPlayerController()->GetPawn()) {
			Cast<AHANSEIRacingController>(GetWorld()->GetFirstPlayerController())->LocationSendTimerStart(this);
			GetWorld()->GetFirstPlayerController()->GetPawn()->Destroy();
		}
		GetWorld()->GetFirstPlayerController()->Possess(NewPawn);
		m_Character = NewPawn;
		NewPawn->SetIsItPlayer(true);
	}
	else {
		NewPawn->SpawnDefaultController();
	}
	if (PlayerName) {
		NewPawn->SetPlayerName(FString(PlayerName));
	}
	NewPawn->SetMaterialFromUniqueKey(UniqueKey);
	NewPawn->SetPlayerRank(m_CharacterClass.Num() + 1);
	m_CharacterClass.Add(TPairInitializer<int32, ADefaultVehicleCharacter*>(UniqueKey, NewPawn));
}

void AInGameMode::SpawnCharacter() {
	if (m_GameInstance && m_GameInstance->IsValidLowLevel() && m_SpawnPoint.Num() >= m_PlayerList.size()) {
		for (size_t i = m_CharacterClass.Num(); i < m_PlayerList.size(); i++) {
			AActor* SpawnPoint = FindSpawnPointByUniqueKey(m_PlayerList[i].m_UniqueKey);

			if (SpawnPoint) {
				FVector Location = SpawnPoint->GetActorLocation();
				FRotator Rotation(0.f, 0.f, 0.f);
				FActorSpawnParameters Param;
				Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				auto NewPawn = GetWorld()->SpawnActor<ADefaultVehicleCharacter>(ADefaultVehicleCharacter::StaticClass(), Location, Rotation, Param);

				if (NewPawn) {
					SpawnPawnAndAddCharacterList(NewPawn, m_PlayerList[i].m_UniqueKey, m_PlayerList[i].m_PlayerNickName);

					if (m_CharacterClass.Num() == m_PlayerList.size()) {
						if (IsValid(m_InGameWidget)) {
							m_InGameWidget->SetCharacterClassData(m_CharacterClass);
						}
						m_bIsSpawnPlayer = false;
					}
				}
			}
		}
	}
}

void AInGameMode::TeleportCharacters() {
	for (int32 i = 0; i < m_CharacterClass.Num(); i++) {
		if (IsValid(m_CharacterClass[i]) && IsValid(m_CharacterClass[i]->GetController()) && i < m_StartPoint.Num() && IsValid(m_StartPoint[i])) {
			m_CharacterClass[i]->TeleportTo(m_StartPoint[i]->GetActorLocation(), m_StartPoint[i]->GetActorRotation());
		}
	}
	m_bIsTeleport = false;
}

void AInGameMode::ResetPlayerCurrentRank(const int32& UniqueKey, const int32& TargetRank) {
	for (auto It : m_CharacterClass) {
		if (It.Key != UniqueKey && It.Value->GetPlayerCurrentRank() > TargetRank) {
			It.Value->SetPlayerRank(It.Value->GetPlayerCurrentRank() - 1);
		}
	}
}

uint8* AInGameMode::RecvBufferShiftProcess(uint8* RecvBuffer, const int32& RecvBytes, const int32& PacketSize) {
	uint8* Result = nullptr;
	Result = (uint8*)std::memmove(RecvBuffer, RecvBuffer + PacketSize, RecvBytes);
	return Result;
}

void AInGameMode::UpdatePlayerLocationAndRotation() {
	if (m_GameInstance && m_GameInstance->IsValidLowLevel()) {
		for (auto It : m_PlayerList) {
			if (It.m_UniqueKey != m_GameInstance->GetUniqueKey()) {
				auto Character = m_CharacterClass.Find(It.m_UniqueKey);
				FVector Location(It.m_Location.x, It.m_Location.y, It.m_Location.z);
				FRotator Rotation(It.m_Rotation.x, It.m_Rotation.y, It.m_Rotation.z);
				if (Character != nullptr) {
					(*Character)->SetActorLocationAndRotation(Location, FQuat(Rotation), false, nullptr, ETeleportType::TeleportPhysics);
					(*Character)->SetVehicleState(It.m_VehicleData);
				}
			}
		}
	}
}