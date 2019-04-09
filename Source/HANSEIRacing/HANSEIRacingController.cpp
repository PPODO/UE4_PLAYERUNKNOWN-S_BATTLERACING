#include "HANSEIRacingController.h"
#include "DefaultVehicleCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"

AHANSEIRacingController::AHANSEIRacingController() : m_Character(nullptr), m_ControllerConnector(nullptr) {

}

void AHANSEIRacingController::BeginPlay() {
	Super::BeginPlay();

	m_ControllerConnector = new FUDPSocketComponent;
	if (m_ControllerConnector && m_ControllerConnector->StartUDPReceiver()) {
		m_ControllerConnector->GetUDPReceiver()->OnDataReceived().BindUObject(this, &AHANSEIRacingController::RecvControllerData);
		m_ControllerConnector->GetUDPReceiver()->Start();
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
	if (m_TimerHandle.IsValid() && GetWorld()) {
		GetWorld()->GetTimerManager().ClearTimer(m_TimerHandle);
	}
}

void AHANSEIRacingController::LocationSendTimerStart(AGameModeBase* GM) {
	GetWorld()->GetTimerManager().SetTimer(m_TimerHandle, this, &AHANSEIRacingController::IsItNearlyEqualActorInformation, 0.016f, true);

	if (GM) {
	}
}

void AHANSEIRacingController::IsItNearlyEqualActorInformation() {
/*	if (IsValid(m_Character) && IsValid(m_GameMode)) {
		PLAYER::VehicleState ActorState = m_Character->GetVehicleState();
		FVector ActorLocation = m_Character->GetActorLocation();
		FVector ActorRotation(m_Character->GetActorRotation().Pitch, m_Character->GetActorRotation().Yaw, m_Character->GetActorRotation().Roll);

		m_Location = ActorLocation; m_Rotation = ActorRotation;
		m_GameMode->SendPlayerInformation(m_Location, m_Rotation, ActorState);
	}*/
}

void AHANSEIRacingController::RecvControllerData(const FArrayReaderPtr& Ptr, const FIPv4Endpoint & EndPoint) {
	*Ptr << m_ControllerData;
}