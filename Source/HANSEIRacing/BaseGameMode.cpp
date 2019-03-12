#include "BaseGameMode.h"
#include "SocketComponent.h"

ABaseGameMode::ABaseGameMode() : m_ClientSocket(nullptr) {
	
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseGameMode::BeginPlay() {
	Super::BeginPlay();

	m_ClientSocket = new FSocketComponent(this, 3510);
}

void ABaseGameMode::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void ABaseGameMode::BeginDestroy() {
	Super::BeginDestroy();

	if (m_ClientSocket) {
		m_ClientSocket->StopThread();
		delete m_ClientSocket;
		m_ClientSocket = nullptr;
	}
}

void ABaseGameMode::RecvDataProcessing(TCHAR* RecvMessage) {
	
}