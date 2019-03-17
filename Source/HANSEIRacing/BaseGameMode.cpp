#include "BaseGameMode.h"
#include "SocketComponent.h"
#include "HANSEIRacingGameInstance.h"

ABaseGameMode::ABaseGameMode() : m_GameInstance(nullptr), m_Socket(nullptr), m_bFailedJoinGame(false) {
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseGameMode::BeginPlay() {
	Super::BeginPlay();

	m_GameInstance = Cast<UHANSEIRacingGameInstance>(GetGameInstance());
	m_Socket = new FSocketComponent;
	if (m_Socket) {
		m_Socket->ConnectToServer(this, m_Port, m_SocketName);
	}
}

void ABaseGameMode::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void ABaseGameMode::BeginDestroy() {
	Super::BeginDestroy();

	if (m_Socket) {
		m_Socket->StopThread();
		delete m_Socket;
		m_Socket = nullptr;
	}
}

void ABaseGameMode::RecvDataProcessing(TCHAR* RecvMessage) {
	
}
