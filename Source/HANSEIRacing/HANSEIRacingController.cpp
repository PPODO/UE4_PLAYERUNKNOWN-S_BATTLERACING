#include "HANSEIRacingController.h"
#include "DefaultVehicleCharacter.h"
#include "DefaultCharacter.h"
#include "InGameMode.h"
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
		m_GameMode = Cast<AInGameMode>(GM);
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