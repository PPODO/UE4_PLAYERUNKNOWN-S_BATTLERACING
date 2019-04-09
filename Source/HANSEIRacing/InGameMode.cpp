#include "InGameMode.h"
#include "HANSEIRacingGameInstance.h"

AInGameMode::AInGameMode() : m_GameInstance(nullptr) {
	m_Port = 3500;
	m_SocketName = L"Login_Socket";

	PrimaryActorTick.bCanEverTick = true;
}

void AInGameMode::BeginPlay() {
	ABaseGameMode::BeginPlay();

	m_GameInstance = Cast<UHANSEIRacingGameInstance>(GetGameInstance());
	if(!m_GameInstance){
		m_GameInstance = Cast<UHANSEIRacingGameInstance>(GetGameInstance());
	}

	SendJoinGameToServer();
}

void AInGameMode::Tick(float DeltaTime) {
	ABaseGameMode::Tick(DeltaTime);

}

void AInGameMode::BeginDestroy() {
	ABaseGameMode::BeginDestroy();

}

void AInGameMode::RecvDataProcessing(uint8* RecvBuffer) {
	Packet = (GAMEPACKET*)RecvBuffer;

	if (Packet) {
		if (Packet->m_MessageType == EPACKETMESSAGEFORGAMETYPE::EPMGT_JOIN) {
			IsSucceedJoinGame(Packet);
		}
	}
	Packet = nullptr;
}

// FROM Server

void AInGameMode::IsSucceedJoinGame(GAMEPACKET *& Packet) {
	if (Packet) {
		UE_LOG(LogTemp, Warning, L"HI : %d", static_cast<int32>(Packet->m_FailedReason));
	}
}

// TO Server

void AInGameMode::SendJoinGameToServer() {
	GAMEPACKET Packet;

	if (GetSocket()) {
		Packet.m_MessageType = EPACKETMESSAGEFORGAMETYPE::EPMGT_JOIN;

		GetSocket()->Send((char*)&Packet, sizeof(GAMEPACKET));
	}
}