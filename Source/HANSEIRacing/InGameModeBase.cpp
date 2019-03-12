#include "InGameModeBase.h"
#include "DefaultCharacter.h"
#include "SocketComponent.h"

AInGameModeBase::AInGameModeBase() : m_ClientSocket(nullptr) {
	DefaultPawnClass = ADefaultCharacter::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
}

void AInGameModeBase::BeginPlay() {
	Super::BeginPlay();

	//m_ClientSocket = new FSocketComponent(3500);
}

void AInGameModeBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AInGameModeBase::BeginDestroy() {
	Super::BeginDestroy();

	if (m_ClientSocket) {
		m_ClientSocket->StopThread();
	}
}