#include "DefaultCharacter.h"
#include "HANSEIRacginPlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ConstructorHelpers.h"
#include "Engine/World.h"

ADefaultCharacter::ADefaultCharacter() : m_bIsDisconnect(false) {
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(L"StaticMesh'/Engine/BasicShapes/Cube.Cube'");

	if (MeshObject.Object) {
		m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Component");
		m_Mesh->SetStaticMesh(MeshObject.Object);
		m_Mesh->SetRelativeLocation(FVector(0.f));
		m_Mesh->SetRelativeRotation(FRotator(0.f));
		m_Mesh->AttachTo(RootComponent);
	}
	
	PrimaryActorTick.bCanEverTick = true;
}

void ADefaultCharacter::BeginPlay() {
	Super::BeginPlay();

}

void ADefaultCharacter::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);

	m_Controller = Cast<AHANSEIRacginPlayerController>(NewController);
	if (!m_Controller) {
		m_Controller = Cast<AHANSEIRacginPlayerController>(NewController);
	}
}

void ADefaultCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (m_bIsDisconnect) {
		Destroy();
	}
}

void ADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Forward", this, &ADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &ADefaultCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &ADefaultCharacter::TurnRate);
	PlayerInputComponent->BindAxis("LookAtRate", this, &ADefaultCharacter::LookAtRate);
}

void ADefaultCharacter::MoveForward(float Value) {
	if (Value != 0.f) {
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}
}

void ADefaultCharacter::MoveRight(float Value) {
	if (Value != 0.f) {
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

void ADefaultCharacter::TurnRate(float Value) {
	if (Value != 0.f) {
		AddControllerYawInput(Value);
	}
}

void ADefaultCharacter::LookAtRate(float Value) {
	if (Value != 0.f) {
		AddControllerPitchInput(Value);
	}
}
