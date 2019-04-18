#include "DefaultCharacter.h"
#include "HANSEIRacingController.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "ConstructorHelpers.h"

ADefaultCharacter::ADefaultCharacter() {
	ConstructorHelpers::FObjectFinder<UStaticMesh> Object(L"StaticMesh'/Engine/BasicShapes/Cube.Cube'");

	if (Object.Succeeded()) {
		m_Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh Object");
		m_Mesh->SetupAttachment(RootComponent);
		m_Mesh->SetStaticMesh(Object.Object);
	}

	m_SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm Component");
	m_SpringArm->SetupAttachment(RootComponent);
	m_SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 34.f));
	m_SpringArm->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f));
	m_SpringArm->TargetArmLength = 125.f;
	m_SpringArm->bUsePawnControlRotation = true;

	m_Camera = CreateDefaultSubobject<UCameraComponent>("Camera Component");
	m_Camera->SetupAttachment(m_SpringArm, USpringArmComponent::SocketName);
	m_Camera->SetRelativeLocation(FVector(-125.f, 0.f, 0.f));
	m_Camera->SetRelativeRotation(FRotator(10.f, 0.f, 0.f));
	m_Camera->FieldOfView = 90.f;
	m_Camera->bUsePawnControlRotation = false;

	PrimaryActorTick.bCanEverTick = true;
}

void ADefaultCharacter::BeginPlay() {
	Super::BeginPlay();

}

void ADefaultCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void ADefaultCharacter::PossessedBy(AController* NewController) {
	m_Controller = Cast<AHANSEIRacingController>(NewController);
	if (!m_Controller) {
		m_Controller = Cast<AHANSEIRacingController>(NewController);
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
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ADefaultCharacter::MoveRight(float Value) {
	if (Value != 0.f) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ADefaultCharacter::TurnRate(float Value) {
	if (Controller && Value != 0.f) {
		AddControllerYawInput(Value);
	}
}

void ADefaultCharacter::LookAtRate(float Value) {
	if (Controller && Value != 0.f) {
		AddControllerPitchInput(Value);
	}
}