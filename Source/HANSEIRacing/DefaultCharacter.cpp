#include "DefaultCharacter.h"
#include "ConstructorHelpers.h"
#include "HANSEIRacginPlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"

ADefaultCharacter::ADefaultCharacter() : m_PlayerController(false) {
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(L"StaticMesh'/Engine/BasicShapes/Cube.Cube'");

	if (MeshObject.Succeeded()) {
		m_PawnMesh = CreateDefaultSubobject<UStaticMeshComponent>(L"Pawn Mesh Component");
		m_PawnMesh->SetStaticMesh(MeshObject.Object);
	}

	PrimaryActorTick.bCanEverTick = true;
}

void ADefaultCharacter::BeginPlay() {
	Super::BeginPlay();

}

void ADefaultCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);

	m_PlayerController = Cast<AHANSEIRacginPlayerController>(NewController);
}

void ADefaultCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void ADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("PauseUI", IE_Pressed, this, &ADefaultCharacter::TogglePauseUI);

	PlayerInputComponent->BindAxis("Forward", this, &ADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &ADefaultCharacter::MoveRight);
}

void ADefaultCharacter::MoveForward(float Value) {

}

void ADefaultCharacter::MoveRight(float Value) {
	
}

void ADefaultCharacter::TogglePauseUI() {
	if (m_PlayerController) {
		m_PlayerController->SetVisiblePauseUI(!m_PlayerController->GetVisiblePauseUI());
		m_PlayerController->bShowMouseCursor = m_PlayerController->GetVisiblePauseUI();

		if (m_PlayerController->GetVisiblePauseUI()) {
			m_PlayerController->SetInputMode(FInputModeGameAndUI());
		}
		else {
			m_PlayerController->SetInputMode(FInputModeGameOnly());
		}
	}
}