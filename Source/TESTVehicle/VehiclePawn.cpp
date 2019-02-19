#include "VehiclePawn.h"
#include "VehicleWheelFront.h"
#include "VehicleWheelRear.h"
#include "WheelAnimInstance.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "ConstructorHelpers.h"

AVehiclePawn::AVehiclePawn() {
	ConstructorHelpers::FObjectFinder<USkeletalMesh> VehicleMeshObject(L"SkeletalMesh'/Game/VehicleMesh/SK_Car.SK_Car'");
	ConstructorHelpers::FClassFinder<UWheelAnimInstance> VehicleAnimObject(L"/Game/Blueprint/BP_WheelAnimInstance");
	if (VehicleMeshObject.Succeeded() && VehicleAnimObject.Succeeded()) {
		GetMesh()->SetSkeletalMesh(VehicleMeshObject.Object);
		GetMesh()->SetAnimInstanceClass(VehicleAnimObject.Class);
	}

	m_SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("Spring Arm Component");
	if (m_SpringArmComp) {
		m_SpringArmComp->SetRelativeLocation(FVector(-475.f, 0.f, 350.f));
		m_SpringArmComp->SetRelativeRotation(FRotator(-25.f, 0.f, 0.f));
		m_SpringArmComp->TargetArmLength = 125.f;
		m_SpringArmComp->bEnableCameraRotationLag = true;
		m_SpringArmComp->bInheritPitch = m_SpringArmComp->bInheritRoll = m_SpringArmComp->bInheritYaw = true;
		m_SpringArmComp->AttachTo(RootComponent);
	}

	m_CameraComp = CreateDefaultSubobject<UCameraComponent>("Camera Component");
	if (m_SpringArmComp && m_CameraComp) {
		m_CameraComp->AttachTo(m_SpringArmComp, USpringArmComponent::SocketName);
		m_CameraComp->SetRelativeLocation(FVector::ZeroVector);
		m_CameraComp->SetRelativeRotation(FRotator::ZeroRotator);
		m_CameraComp->bUsePawnControlRotation = false;
		m_CameraComp->FieldOfView = 90.f;
	}

	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());
	if (Vehicle4W) {
		Vehicle4W->WheelSetups[0].WheelClass = UVehicleWheelFront::StaticClass();
		Vehicle4W->WheelSetups[0].BoneName = "Tire_frontleft";
		
		Vehicle4W->WheelSetups[1].WheelClass = UVehicleWheelFront::StaticClass();
		Vehicle4W->WheelSetups[1].BoneName = "Tire_frontright";

		Vehicle4W->WheelSetups[2].WheelClass = UVehicleWheelRear::StaticClass();
		Vehicle4W->WheelSetups[2].BoneName = "Tire_rearleft";

		Vehicle4W->WheelSetups[3].WheelClass = UVehicleWheelRear::StaticClass();
		Vehicle4W->WheelSetups[3].BoneName = "Tire_rearright";
	}
}

void AVehiclePawn::BeginPlay() {
	Super::BeginPlay();

}

void AVehiclePawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void AVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("HandBreak", IE_Pressed, this, &AVehiclePawn::PressedHandBreak);
	PlayerInputComponent->BindAction("HandBreak", IE_Released, this, &AVehiclePawn::ReleaseHandBreak);

	PlayerInputComponent->BindAxis("MoveForward", this, &AVehiclePawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AVehiclePawn::MoveRight);
}

void AVehiclePawn::MoveForward(float Value) {
	if (GetVehicleMovement()) {
		GetVehicleMovement()->SetThrottleInput(Value);
	}
}

void AVehiclePawn::MoveRight(float Value) {
	if (GetVehicleMovement()) {
		GetVehicleMovement()->SetSteeringInput(Value);
	}
}

void AVehiclePawn::PressedHandBreak() {
	if (GetVehicleMovement()) {
		GetVehicleMovement()->SetHandbrakeInput(true);
	}
}

void AVehiclePawn::ReleaseHandBreak() {
	if (GetVehicleMovement()) {
		GetVehicleMovement()->SetHandbrakeInput(false);
	}
}