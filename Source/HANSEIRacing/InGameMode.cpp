#include "InGameMode.h"
#include "ItemSpawner.h"
#include "InGameWidget.h"
#include "LobbyWidget.h"
#include "HANSEIRacingController.h"
#include "HANSEIRacingGameInstance.h"
#include "DefaultVehicleCharacter.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "StartPoint.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include <algorithm>

AInGameMode::AInGameMode() : m_GameInstance(nullptr), m_InGameWidget(nullptr), m_LobbyWidget(nullptr), m_Character(nullptr), m_bIsSpawnPlayer(false), m_bIsLeader(false), m_bIsReady(false), m_SocketNumber(0), m_bIsInGame(false), m_bIsTeleport(false) {
	PlayerControllerClass = AHANSEIRacingController::StaticClass();

	m_Port = 3500;
	m_SocketName = L"Login_Socket";

	PrimaryActorTick.bCanEverTick = true;
}

void AInGameMode::BeginPlay() {
	ABaseGameMode::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), m_SpawnPoint);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStartPoint::StaticClass(), m_StartPoint);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemSpawner::StaticClass(), m_ItemSpawners);

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
	PACKET* Packet = reinterpret_cast<PACKET*>(RecvBuffer);
	if (Packet) {
		int32 PacketSize = CalculatePacketSize(Packet);
		if (PacketSize > 0) {
			m_PacketQueue.push(Packet);
			for (int32 i = 1; i < (RecvBytes / PacketSize); i++) {
				uint8* ShiftBuffer = RecvBufferShiftProcess(RecvBuffer, PacketSize, i);
				PACKET* Pack = reinterpret_cast<PACKET*>(ShiftBuffer);
				if (Pack) {
					m_PacketQueue.push(Pack);
					PacketSize = CalculatePacketSize(m_PacketQueue.front());
					if (PacketSize <= 0) {
						break;
					}
				}
			}
		}
	}
	
	while (!m_PacketQueue.empty()) {
		PACKET* Packet = m_PacketQueue.front();

		switch (Packet->m_MessageType) {
		case EPACKETMESSAGEFORGAMETYPE::EPMGT_JOIN:
			IsSucceedJoinGame(*static_cast<GAMEPACKET*>(Packet));
			break;
		case EPACKETMESSAGEFORGAMETYPE::EPMGT_NEWPLAYER:
			IsSucceedJoinGameNewPlayer(*static_cast<GAMEPACKET*>(Packet));
			break;
		case EPACKETMESSAGEFORGAMETYPE::EPMGT_DISCONNECTOTHER:
			IsSucceedDisconnectOtherPlayer(*static_cast<GAMEPACKET*>(Packet));
			break;
		case EPACKETMESSAGEFORGAMETYPE::EPMGT_UPDATE:
			IsSucceedUpdatePlayerInformation(*static_cast<GAMEPACKET*>(Packet));
			break;
		case EPACKETMESSAGEFORGAMETYPE::EPMGT_STARTGAME:
			IsSucceedStartGame(*static_cast<GAMEPACKET*>(Packet));
			break;
		case EPACKETMESSAGEFORGAMETYPE::EPMGT_READY:
			IsSucceedChangeReadyState(*static_cast<GAMEPACKET*>(Packet));
			break;
		case EPACKETMESSAGEFORGAMETYPE::EPMGT_SPAWNITEM:
			IsSucceedRespawnItem(*static_cast<SPAWNERPACKET*>(Packet));
			break;
		}
		m_PacketQueue.pop();
	}
}

// FROM Server

void AInGameMode::IsSucceedJoinGame(GAMEPACKET& Packet) {
	if (Packet.m_FailedReason == EPACKETFAILEDTYPE::EPFT_SUCCEED) {
		if (m_GameInstance && m_GameInstance->IsValidLowLevel()) {
			m_GameInstance->SetUniqueKey(Packet.m_UniqueKey);
		}
		m_bIsLeader = Packet.m_bIsLeader;
		m_SocketNumber = Packet.m_Socket;
		m_PlayerList.push_back(Packet);
		m_bIsSpawnPlayer = true;

		if (IsValid(m_LobbyWidget)) {
			m_LobbyWidget->SetPlayerList(m_PlayerList);
		}
	}
}

void AInGameMode::IsSucceedJoinGameNewPlayer(GAMEPACKET& Packet) {
	if (Packet.m_FailedReason == EPACKETFAILEDTYPE::EPFT_SUCCEED) {
		m_PlayerList.push_back(Packet);
		m_bIsSpawnPlayer = true;

		if (IsValid(m_LobbyWidget)) {
			m_LobbyWidget->SetPlayerList(m_PlayerList);
		}
	}
}

void AInGameMode::IsSucceedDisconnectOtherPlayer(GAMEPACKET& Packet) {
	if (Packet.m_FailedReason == EPACKETFAILEDTYPE::EPFT_SUCCEED) {
		auto Character = m_CharacterClass.Find(Packet.m_UniqueKey);
		auto CharacterList = std::find_if(m_PlayerList.begin(), m_PlayerList.end(), [&](const GAMEPACKET& Data) -> bool { if (Data.m_UniqueKey == Packet.m_UniqueKey) { return true; } return false; });
		if (Character != nullptr && CharacterList != m_PlayerList.cend()) {
			(*Character)->SetIsDisconnect(true);
			m_PlayerList.erase(CharacterList);
			m_CharacterClass.Remove(Packet.m_UniqueKey);
		}

		// Update Player List
		if (m_bIsInGame) {
			if (IsValid(m_InGameWidget)) {
				m_InGameWidget->SetCharacterClassData(m_CharacterClass);
			}
		}
		else {
			if (IsValid(m_LobbyWidget)) {
				m_LobbyWidget->SetPlayerList(m_PlayerList);
			}
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

void AInGameMode::IsSucceedChangeReadyState(GAMEPACKET& Packet) {
	if (!m_bIsInGame) {
		if (Packet.m_FailedReason == EPACKETFAILEDTYPE::EPFT_SUCCEED) {
			auto Character = std::find_if(m_PlayerList.begin(), m_PlayerList.end(), [&](const GAMEPACKET& Data) -> bool { if (Data.m_UniqueKey == Packet.m_UniqueKey) { return true; } return false; });
			if (Character != m_PlayerList.cend() && IsValid(m_LobbyWidget)) {
				if (Character->m_UniqueKey == m_GameInstance->GetUniqueKey()) {
					m_bIsReady = Packet.m_bIsReady;
				}
				Character->m_bIsReady = Packet.m_bIsReady;
				m_LobbyWidget->SetPlayerList(m_PlayerList);
			}
		}
	}
}

void AInGameMode::IsSucceedRespawnItem(SPAWNERPACKET& Packet) {
	if (Packet.m_FailedReason == EPACKETFAILEDTYPE::EPFT_SUCCEED) {
		if (Packet.m_ItemInformation.m_SpawnerID >= 0 && Packet.m_ItemInformation.m_SpawnerID < m_ItemSpawners.Num()) {
			auto SpawnerActor = Cast<AItemSpawner>(*m_ItemSpawners.FindByPredicate([&](AActor* Actor) -> bool { if (IsValid(Actor) && IsValid(Cast<AItemSpawner>(Actor))) {
				if (Cast<AItemSpawner>(Actor)->GetSpawnerUniqueID() == Packet.m_ItemInformation.m_SpawnerID) {
					return true;
				}
			} return false; }));
			if (SpawnerActor && IsValid(SpawnerActor)) {
				SpawnerActor->FindItemIndexAndReset(Packet.m_ItemInformation.m_Index);
			}
		}
	}
}

// TO Server

void AInGameMode::SendJoinGameToServer() {
	GAMEPACKET Packet;
	memset(&Packet, 0, sizeof(Packet));

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
	RANK PlayerRankInformation;
	memset(&Packet, 0, sizeof(Packet));

	if (GetSocket() && m_Character && m_GameInstance && m_GameInstance->IsValidLowLevel()) {
		PlayerRankInformation.m_CurrentSplinePoint = m_Character->m_CurrentSplinePoint;
		PlayerRankInformation.m_SplinePointDistance = m_Character->m_SplinePointDistance;
		
		Packet.m_RankInformation = PlayerRankInformation;
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
	memset(&Packet, 0, sizeof(Packet));

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
		memset(&Packet, 0, sizeof(Packet));

		if (GetSocket() && IsValid(m_GameInstance)) {
			Packet.m_PacketType = EPACKETTYPE::EPT_PLAYER;
			Packet.m_MessageType = EPACKETMESSAGEFORGAMETYPE::EPMGT_STARTGAME;
			Packet.m_SessionID = m_GameInstance->GetSessionID();
			Packet.m_Socket = m_SocketNumber;

			GetSocket()->Send((ANSICHAR*)&Packet, sizeof(GAMEPACKET));
		}
	}
}

void AInGameMode::SendChangeReadyState() {
	if (!m_bIsInGame) {
		GAMEPACKET Packet;
		memset(&Packet, 0, sizeof(Packet));

		if (GetSocket() && IsValid(m_GameInstance)) {
			Packet.m_PacketType = EPACKETTYPE::EPT_PLAYER;
			Packet.m_MessageType = EPACKETMESSAGEFORGAMETYPE::EPMGT_READY;
			Packet.m_SessionID = m_GameInstance->GetSessionID();
			Packet.m_UniqueKey = m_GameInstance->GetUniqueKey();
			Packet.m_bIsReady = !m_bIsReady;
			Packet.m_Socket = m_SocketNumber;

			GetSocket()->Send((ANSICHAR*)&Packet, sizeof(GAMEPACKET));
		}
	}
}

void AInGameMode::SendRespawnItemToServer(const int32 & SpawnerID, const int32 & ItemIndex) {
	GAMEPACKET Packet;
	memset(&Packet, 0, sizeof(Packet));

	if (GetSocket() && IsValid(m_GameInstance)) {
		Packet.m_PacketType = EPACKETTYPE::EPT_SPAWNER;
		Packet.m_MessageType = EPACKETMESSAGEFORGAMETYPE::EPMGT_SPAWNITEM;
		Packet.m_SessionID = m_GameInstance->GetSessionID();
		Packet.m_ItemInformation.m_bIsActivated = true;
		Packet.m_ItemInformation.m_SpawnerID = SpawnerID;
		Packet.m_ItemInformation.m_Index = ItemIndex;
		Packet.m_Socket = m_SocketNumber;

		GetSocket()->Send((ANSICHAR*)&Packet, sizeof(GAMEPACKET));
	}
}

// Private Function

int32 AInGameMode::CalculatePacketSize(const PACKET* Packet) {
	switch (Packet->m_PacketType) {
	case EPACKETTYPE::EPT_PLAYER:
		return sizeof(GAMEPACKET);
	case EPACKETTYPE::EPT_SPAWNER:
		return sizeof(SPAWNERPACKET);
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

void AInGameMode::SpawnPawnAndAddCharacterList(ADefaultVehicleCharacter* NewPawn, const int32& UniqueKey, const ANSICHAR* PlayerName, const int32& PlayerRank) {
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
	NewPawn->SetPlayerRank(PlayerRank);
	m_CharacterClass.Add(TPairInitializer<int32, ADefaultVehicleCharacter*>(UniqueKey, NewPawn));
}

void AInGameMode::SpawnCharacter() {
	if (m_GameInstance && m_GameInstance->IsValidLowLevel() && m_SpawnPoint.Num() >= m_PlayerList.size()) {
		for (int32 i = m_CharacterClass.Num(); i < m_PlayerList.size(); i++) {
			AActor* SpawnPoint = FindSpawnPointByUniqueKey(m_PlayerList[i].m_UniqueKey);

			if (SpawnPoint) {
				FVector Location = SpawnPoint->GetActorLocation();
				FRotator Rotation(0.f, 0.f, 0.f);
				FActorSpawnParameters Param;
				Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				auto NewPawn = GetWorld()->SpawnActor<ADefaultVehicleCharacter>(ADefaultVehicleCharacter::StaticClass(), Location, Rotation, Param);

				if (NewPawn) {
					SpawnPawnAndAddCharacterList(NewPawn, m_PlayerList[i].m_UniqueKey, m_PlayerList[i].m_PlayerNickName, m_PlayerList[i].m_RankInformation.m_CurrentRank);

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
		if (IsValid(m_CharacterClass[i])) {
			const int32* UniqueKey = m_CharacterClass.FindKey(m_CharacterClass[i]);
			if (UniqueKey) {
				auto StartPoint = m_StartPoint.FindByPredicate([&](AActor* Actor) -> bool { if (IsValid(Cast<AStartPoint>(Actor)) && Cast<AStartPoint>(Actor)->m_StartPointTag == *UniqueKey) { return true; } return false; });
				if (StartPoint && IsValid(*StartPoint)) {
					m_CharacterClass[i]->TeleportTo((*StartPoint)->GetActorLocation(), (*StartPoint)->GetActorRotation());
				}
			}
		}
	}
	m_bIsTeleport = false;
}

uint8* AInGameMode::RecvBufferShiftProcess(uint8* RecvBuffer, const int32& PacketSize, const int32& CurrentCount) {
	return RecvBuffer + (PacketSize * CurrentCount);
}

void AInGameMode::UpdatePlayerLocationAndRotation() {
	if (m_GameInstance && m_GameInstance->IsValidLowLevel()) {
		bool bIsUpdateRank = false;

		for (auto It : m_PlayerList) {
			auto Character = m_CharacterClass.Find(It.m_UniqueKey);
			if (Character && IsValid(*Character)) {
				if (It.m_UniqueKey != m_GameInstance->GetUniqueKey()) {
					FVector Location(It.m_Location.x, It.m_Location.y, It.m_Location.z);
					FRotator Rotation(It.m_Rotation.x, It.m_Rotation.y, It.m_Rotation.z);

					(*Character)->SetActorLocationAndRotation(Location, FQuat(Rotation), false, nullptr, ETeleportType::TeleportPhysics);
					(*Character)->SetVehicleState(It.m_VehicleData);
				}

				if ((*Character)->GetPlayerCurrentRank() != It.m_RankInformation.m_CurrentRank) {
					UE_LOG(LogTemp, Warning, L"QWE");
					(*Character)->SetPlayerRank(It.m_RankInformation.m_CurrentRank);
					bIsUpdateRank = true;
				}
			}
		}

		if (bIsUpdateRank && IsValid(m_InGameWidget)) {
			m_InGameWidget->SetCharacterClassData(m_CharacterClass);
		}
	}
}