#include "HANSEIRacingController.h"
#include "DefaultVehicleCharacter.h"
#include "DefaultCharacter.h"
#include "InGameMode.h"
#include "Components/SplineComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

AHANSEIRacingController::AHANSEIRacingController() : m_Character(nullptr), m_ControllerConnector(nullptr), m_GameMode(nullptr), m_CurrentLap(1), m_CurrentSplinePoint(0), m_SplinePointDistance(0.f) {
	
}

void AHANSEIRacingController::BeginPlay() {
	Super::BeginPlay();

	m_ControllerConnector = new FUDPSocketComponent;
	if (m_ControllerConnector && m_ControllerConnector->StartUDPReceiver()) {
		m_ControllerConnector->GetUDPReceiver()->OnDataReceived().BindUObject(this, &AHANSEIRacingController::RecvControllerData);
		m_ControllerConnector->GetUDPReceiver()->Start();
	}

	m_GameMode = Cast<AInGameMode>(GetWorld()->GetAuthGameMode());
	if(!IsValid(m_GameMode)){
		m_GameMode = Cast<AInGameMode>(GetWorld()->GetAuthGameMode());
	}
}

void AHANSEIRacingController::Possess(APawn* Pawn) {
	Super::Possess(Pawn);

	m_Character = Cast<ADefaultVehicleCharacter>(Pawn);
	if (!m_Character) {
		m_Character = Cast<ADefaultVehicleCharacter>(Pawn);
	}
}

void AHANSEIRacingController::BeginDestroy() {
	Super::BeginDestroy();

	if (m_ControllerConnector) {
		delete m_ControllerConnector;
		m_ControllerConnector = nullptr;
	}
	StopLocationSendTimer();
}

void AHANSEIRacingController::StartLocationSendTimer() {
	GetWorld()->GetTimerManager().SetTimer(m_TimerHandle, this, &AHANSEIRacingController::IsItNearlyEqualActorInformation, 0.016f, true);
}

void AHANSEIRacingController::StopLocationSendTimer() {
	if (m_TimerHandle.IsValid() && GetWorld()) {
		GetWorld()->GetTimerManager().ClearTimer(m_TimerHandle);
	}
}

void AHANSEIRacingController::IsItNearlyEqualActorInformation() {
	if (IsValid(m_Character) && IsValid(m_GameMode)) {
		FVector ActorLocation = m_Character->GetActorLocation();
		FRotator ActorRotation = m_Character->GetActorRotation();

		m_GameMode->SendCharacterInformationToServer(ActorLocation, ActorRotation, m_ControllerData);
	}
}

void AHANSEIRacingController::RecvControllerData(const FArrayReaderPtr& Ptr, const FIPv4Endpoint & EndPoint) {
	*Ptr << m_ControllerData;
}

void AHANSEIRacingController::ProcessingOverlapEvent(const int32& NumOfSplinePoint) {
	if (m_CurrentSplinePoint + 1 > NumOfSplinePoint - 1) {
		if (m_CurrentLap + 1 > MaxLapCount) {
			// m_GameMode
		}
		m_CurrentLap++;
	}
	m_CurrentSplinePoint = (m_CurrentSplinePoint + 1) % NumOfSplinePoint;
}

int32 AHANSEIRacingController::CalculateSplineDistanceFromSplineComponent(USplineComponent* SplineClass) {
	if (IsValid(SplineClass) && IsValid(GetPawn())) {
		int32 NumOfSplinePoint = SplineClass->GetNumberOfSplinePoints();
		FVector NextLocation = SplineClass->GetLocationAtSplinePoint(m_CurrentSplinePoint, ESplineCoordinateSpace::World);
		FVector CurrentLocation = SplineClass->GetLocationAtSplinePoint(((m_CurrentSplinePoint - 1) + NumOfSplinePoint) % NumOfSplinePoint, ESplineCoordinateSpace::World);
		FVector CharacterLocation = SplineClass->FindLocationClosestToWorldLocation(GetPawn()->GetActorLocation(), ESplineCoordinateSpace::World);

		FVector SplineAndCharacterDistance = NextLocation - CharacterLocation;
		FVector SplineDistance = NextLocation - CurrentLocation;
		m_SplinePointDistance = FVector::DotProduct(SplineAndCharacterDistance, SplineDistance) / SplineDistance.SizeSquared();
	}
	return m_CurrentSplinePoint;
}