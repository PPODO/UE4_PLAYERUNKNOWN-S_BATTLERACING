#include "HANSEIRacginPlayerController.h"
#include "InGameModeBase.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "TimerManager.h"

AHANSEIRacginPlayerController::AHANSEIRacginPlayerController() : m_Character(nullptr), m_GameMode(nullptr) {

}

void AHANSEIRacginPlayerController::BeginPlay() {
	Super::BeginPlay();

}

void AHANSEIRacginPlayerController::Possess(APawn* Pawn) {
	Super::Possess(Pawn);

	m_Character = Cast<ACharacter>(Pawn);
	if(!m_Character){
		m_Character = Cast<ACharacter>(Pawn);
	}
}

void AHANSEIRacginPlayerController::BeginDestroy() {
	Super::BeginDestroy();

	if (m_TimerHandle.IsValid() && GetWorld()) {
		GetWorld()->GetTimerManager().ClearTimer(m_TimerHandle);
	}
}

void AHANSEIRacginPlayerController::LocationSendTimerStart(AGameModeBase* GM) {
	GetWorld()->GetTimerManager().SetTimer(m_TimerHandle, this, &AHANSEIRacginPlayerController::IsItNearlyEqualLocationAndRotation, 0.016f, true);

	if (GM) {
		m_GameMode = Cast<AInGameModeBase>(GM);
	}
}

void AHANSEIRacginPlayerController::IsItNearlyEqualLocationAndRotation() {
	if (IsValid(m_Character) && IsValid(m_GameMode)) {
		FVector ActorLocation = m_Character->GetActorLocation();
		FVector ActorRotation(m_Character->GetActorRotation().Pitch, m_Character->GetActorRotation().Yaw, m_Character->GetActorRotation().Roll);
		
		if (Location != ActorLocation || Rotation != ActorRotation) {
			Location = ActorLocation;
			Rotation = ActorRotation;
			m_GameMode->SendPlayerLocationAndRotation(Location, Rotation);
		}
	}
}