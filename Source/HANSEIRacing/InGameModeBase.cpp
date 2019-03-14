#include "InGameModeBase.h"
#include "DefaultCharacter.h"
#include "SocketComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "HANSEIRacingGameInstance.h"
#include "HANSEIRacginPlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

AInGameModeBase::AInGameModeBase() : m_bSpawnNewPlayer(false), m_MyCharacter(nullptr), m_PlayerController(nullptr), m_GameInstance(nullptr) {
	PlayerControllerClass = AHANSEIRacginPlayerController::StaticClass();

	m_Port = 3500;
	m_SocketName = "InGameSocket";
	PrimaryActorTick.bCanEverTick = true;
}

void AInGameModeBase::BeginPlay() {
	ABaseGameMode::BeginPlay();

	m_PlayerController = GetWorld()->GetFirstPlayerController();
	m_GameInstance = Cast<UHANSEIRacingGameInstance>(GetGameInstance());
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), m_SpawnPoints);
}

void AInGameModeBase::Tick(float DeltaTime) {
	ABaseGameMode::Tick(DeltaTime);

	if (IsInGameThread()) {
		if (m_bSpawnNewPlayer) {
			SpawnCharacter();
		}

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
	case GAMEPACKET::PM_JOIN:
		IsSucceedJoinSession(RecvStream);
		break;
	case GAMEPACKET::PM_DISCONNECT:

		break;
	}
}

void AInGameModeBase::IsSucceedJoinSession(std::stringstream& Stream) {
	int32 IsSucceed = -1;
	Stream >> IsSucceed;

	if (IsSucceed == GAMEPACKET::EJS_SUCCEDD) {
		m_bSpawnNewPlayer = true;
	}
}

void AInGameModeBase::SendDisconnect() {
	std::stringstream SendStream;

	if (m_GameInstance) {
		SendStream << GAMEPACKET::PM_DISCONNECT << std::endl << TCHAR_TO_ANSI(*m_GameInstance->GetSessionName()) << std::endl;
	}
}

void AInGameModeBase::SpawnCharacter() {
	if (m_PlayerController && m_PlayerController->GetPawn()) {
		m_PlayerController->GetPawn()->Destroy();
	}
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto NewPawn = GetWorld()->SpawnActor<ADefaultCharacter>(ADefaultCharacter::StaticClass(), FVector(0.f), FRotator(0.f), Params);
	if (!m_MyCharacter && NewPawn) {
		m_PlayerController->Possess(NewPawn);
		m_MyCharacter = NewPawn;
	}
	else {
		NewPawn->SpawnDefaultController();
	}
	m_bSpawnNewPlayer = false;
}