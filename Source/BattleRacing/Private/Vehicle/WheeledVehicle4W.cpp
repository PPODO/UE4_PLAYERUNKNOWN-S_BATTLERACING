#include "Vehicle/WheeledVehicle4W.h"
#include "Vehicle/WheelFront.h"
#include "Vehicle/WheelRear.h"
#include "Components/AudioComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Sound/SoundCue.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Engine.h"

AWheeledVehicle4W::AWheeledVehicle4W() {
	::ConstructorHelpers::FObjectFinder<USoundCue> engineSound(L"SoundCue'/Game/Sound/Engine_Loop_Cue.Engine_Loop_Cue'");
	::ConstructorHelpers::FObjectFinder<USkeletalMesh> vehicleMesh(L"SkeletalMesh'/Game/Meshes/Vehicle/Vehicle_SkelMesh.Vehicle_SkelMesh'");
	::ConstructorHelpers::FClassFinder<UAnimInstance> vehicleAnim(L"'/Game/Animations/AnimInstance/VehicleAnimationBlueprint'");

	if (vehicleMesh.Succeeded()) {
		GetMesh()->SetSkeletalMesh(vehicleMesh.Object);
		GetMesh()->SetAnimInstanceClass(vehicleAnim.Class);
		GetMesh()->SetCenterOfMass(FVector(-0.f, 0.f, -0.f));
	}
	if (engineSound.Succeeded()) {
		mEngineSoundComponent = CreateDefaultSubobject<UAudioComponent>(L"Engine Sound Component");
		mEngineSoundComponent->SetupAttachment(RootComponent);
		mEngineSoundComponent->SetSound(engineSound.Object);
	}

	mSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(L"Spring Arm Component");
	mSpringArmComponent->SetRelativeLocation(FVector(-75.f, 0.f, 100.f));
	mSpringArmComponent->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	mSpringArmComponent->TargetArmLength = 125.f;
	mSpringArmComponent->bDoCollisionTest = false;
	mSpringArmComponent->bEnableCameraLag = false;
	mSpringArmComponent->bEnableCameraRotationLag = false;
	mSpringArmComponent->bInheritPitch = true;
	mSpringArmComponent->bInheritRoll = true;
	mSpringArmComponent->bInheritYaw = true;
	mSpringArmComponent->SetupAttachment(RootComponent);

	mCameraComponent = CreateDefaultSubobject<UCameraComponent>(L"Camera Component");
	mCameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	mCameraComponent->SetRelativeRotation(FRotator(350.f, 0.f, 0.f));
	mCameraComponent->FieldOfView = 90.f;
	mCameraComponent->bUsePawnControlRotation = false;
	mCameraComponent->SetupAttachment(mSpringArmComponent, USpringArmComponent::SocketName);

	InitializeVehicle();
}

void AWheeledVehicle4W::BeginPlay() {
	Super::BeginPlay();

}

void AWheeledVehicle4W::Tick(float deltaTime) {
	Super::Tick(deltaTime);

	CalculateEngineSound();
}

void AWheeledVehicle4W::SetupPlayerInputComponent(UInputComponent* playerInputComponent) {
	Super::SetupPlayerInputComponent(playerInputComponent);

	playerInputComponent->BindAxis(L"MoveForward", this, &AWheeledVehicle4W::MoveForward);
	playerInputComponent->BindAxis(L"MoveRight", this, &AWheeledVehicle4W::MoveRight);

	playerInputComponent->BindAction(L"Handbrake", EInputEvent::IE_Pressed, this, &AWheeledVehicle4W::PressedBrake);
	playerInputComponent->BindAction(L"Handbrake", EInputEvent::IE_Released, this, &AWheeledVehicle4W::ReleasedBrake);
}

void AWheeledVehicle4W::MoveForward(float val) {
	GetVehicleMovement()->SetThrottleInput(val);
}

void AWheeledVehicle4W::MoveRight(float val) {
	GetVehicleMovement()->SetSteeringInput(val);
}

void AWheeledVehicle4W::PressedBrake() {
	GetVehicleMovement()->SetHandbrakeInput(true);
}

void AWheeledVehicle4W::ReleasedBrake() {
	GetVehicleMovement()->SetHandbrakeInput(false);
}

void AWheeledVehicle4W::InitializeVehicle() {
	auto vehicleMovement = Cast<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	check(vehicleMovement->WheelSetups.Num() == 4);

	vehicleMovement->WheelSetups[0].WheelClass = UWheelFront::StaticClass();
	vehicleMovement->WheelSetups[0].BoneName = "PhysWheel_FL";
	vehicleMovement->WheelSetups[0].AdditionalOffset = FVector(0.f, -8.f, 0.f);

	vehicleMovement->WheelSetups[1].WheelClass = UWheelFront::StaticClass();
	vehicleMovement->WheelSetups[1].BoneName = "PhysWheel_FR";
	vehicleMovement->WheelSetups[1].AdditionalOffset = FVector(0.f, 8.f, 0.f);

	vehicleMovement->WheelSetups[2].WheelClass = UWheelRear::StaticClass();
	vehicleMovement->WheelSetups[2].BoneName = "PhysWheel_BL";
	vehicleMovement->WheelSetups[2].AdditionalOffset = FVector(0.f, -8.f, 0.f);

	vehicleMovement->WheelSetups[3].WheelClass = UWheelRear::StaticClass();
	vehicleMovement->WheelSetups[3].BoneName = "PhysWheel_BR";
	vehicleMovement->WheelSetups[3].AdditionalOffset = FVector(0.f, 8.f, 0.f);

	vehicleMovement->MinNormalizedTireLoad = 0.f;
	vehicleMovement->MinNormalizedTireLoadFiltered = 0.2f;
	vehicleMovement->MaxNormalizedTireLoad = 2.f;
	vehicleMovement->MaxNormalizedTireLoadFiltered = 2.0f;

	vehicleMovement->MaxEngineRPM = 6000.f;
	vehicleMovement->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	vehicleMovement->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.f, 400.f);
	vehicleMovement->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1890.f, 500.f);
	vehicleMovement->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5730.f, 400.f);

	vehicleMovement->SteeringCurve.GetRichCurve()->Reset();
	vehicleMovement->SteeringCurve.GetRichCurve()->AddKey(0.f, 1.f);
	vehicleMovement->SteeringCurve.GetRichCurve()->AddKey(30.f, 0.7f);
	vehicleMovement->SteeringCurve.GetRichCurve()->AddKey(120.f, 0.5f);

	vehicleMovement->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_4W;

	vehicleMovement->DifferentialSetup.FrontRearSplit = 0.65f;

	vehicleMovement->bDeprecatedSpringOffsetMode = true;
	vehicleMovement->TransmissionSetup.bUseGearAutoBox = true;
	vehicleMovement->TransmissionSetup.GearSwitchTime = 0.15;
	vehicleMovement->TransmissionSetup.GearAutoBoxLatency = 1.f;

	UPrimitiveComponent* UpdatedPrimitive = Cast<UPrimitiveComponent>(vehicleMovement->UpdatedComponent);
	if (UpdatedPrimitive) {
		UpdatedPrimitive->BodyInstance.COMNudge = FVector(8.f, 0.f, 0.f);
	}
	vehicleMovement->InertiaTensorScale = FVector(1.f, 1.333f, 1.2f);
}

void AWheeledVehicle4W::CalculateEngineSound() {
	if (IsValid(mEngineSoundComponent)) {
		auto AudioScale = 2500.f / GetVehicleMovement()->GetEngineMaxRotationSpeed();
		mEngineSoundComponent->SetFloatParameter("RPM", GetVehicleMovement()->GetEngineRotationSpeed() * AudioScale);
	}
}