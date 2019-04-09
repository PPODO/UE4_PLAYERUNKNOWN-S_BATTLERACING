#include "BaseGameMode.h"

ABaseGameMode::ABaseGameMode() : m_Socket(nullptr), m_bIsConnected(false) {

	PrimaryActorTick.bCanEverTick = true;
}

void ABaseGameMode::BeginPlay() {
	Super::BeginPlay();

	m_Socket = new FSocketComponent;
	if (!m_Socket) {
		m_Socket = new FSocketComponent;
	}
	else {
		m_Socket->ConnectToServer(this, m_Port, m_SocketName);
	}
}

void ABaseGameMode::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (m_Socket && m_bIsConnected != (m_Socket->GetConnectionState() != ESocketConnectionState::SCS_Connected ? false : true)) {
		m_bIsConnected = (m_Socket->GetConnectionState() != ESocketConnectionState::SCS_Connected ? false : true);
	}
}

void ABaseGameMode::BeginDestroy() {
	Super::BeginDestroy();

	if (m_Socket) {
		m_Socket->StopThread();
		delete m_Socket;
		m_Socket = nullptr;
	}
}

void ABaseGameMode::RecvDataProcessing(uint8 * RecvBuffer) {
}