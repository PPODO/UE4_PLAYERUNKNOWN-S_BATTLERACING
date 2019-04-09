#include "DefaultVehicleCharacter.h"
#include "FrontVehicleWheel.h"
#include "RearVehicleWheel.h"
#include "HANSEIRacingController.h"
#include "WheeledVehicleMovementComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "ConstructorHelpers.h"

ADefaultVehicleCharacter::ADefaultVehicleCharacter() : m_Controller(nullptr) {
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObject(L"SkeletalMesh'/Game/Vehicle/Vehicle_SkelMesh.Vehicle_SkelMesh'");
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstance(L"'/Game/Vehicle/VehicleAnimationBlueprint'");

	if (MeshObject.Succeeded() && AnimInstance.Succeeded()) {
		GetMesh()->SetSkeletalMesh(MeshObject.Object);
		GetMesh()->SetAnimInstanceClass(AnimInstance.Class);
	}

	UWheeledVehicleMovementComponent4W* VehicleWheel = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	check(VehicleWheel->WheelSetups.Num() == 4);

	VehicleWheel->WheelSetups[0].WheelClass = UFrontVehicleWheel::StaticClass();
	VehicleWheel->WheelSetups[0].BoneName = "PhysWheel_FL";
	VehicleWheel->WheelSetups[0].AdditionalOffset = FVector(0.f, -8.f, 0.f);

	VehicleWheel->WheelSetups[1].WheelClass = UFrontVehicleWheel::StaticClass();
	VehicleWheel->WheelSetups[1].BoneName = "PhysWheel_FR";
	VehicleWheel->WheelSetups[1].AdditionalOffset = FVector(0.f, 8.f, 0.f);

	VehicleWheel->WheelSetups[2].WheelClass = URearVehicleWheel::StaticClass();
	VehicleWheel->WheelSetups[2].BoneName = "PhysWheel_BL";
	VehicleWheel->WheelSetups[2].AdditionalOffset = FVector(0.f, -8.f, 0.f);

	VehicleWheel->WheelSetups[3].WheelClass = URearVehicleWheel::StaticClass();
	VehicleWheel->WheelSetups[3].BoneName = "PhysWheel_BR";
	VehicleWheel->WheelSetups[3].AdditionalOffset = FVector(0.f, 8.f, 0.f);

	VehicleWheel->MinNormalizedTireLoad = 0.f;
	VehicleWheel->MinNormalizedTireLoadFiltered = 0.2f;
	VehicleWheel->MaxNormalizedTireLoad = 2.f;
	VehicleWheel->MaxNormalizedTireLoadFiltered = 2.0f;

	VehicleWheel->MaxEngineRPM = 5700.f;
	VehicleWheel->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	VehicleWheel->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.f, 400.f);
	VehicleWheel->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1890.f, 500.f);
	VehicleWheel->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5730.f, 400.f);

	VehicleWheel->SteeringCurve.GetRichCurve()->Reset();
	VehicleWheel->SteeringCurve.GetRichCurve()->AddKey(0.f, 1.f);
	VehicleWheel->SteeringCurve.GetRichCurve()->AddKey(40.f, 0.7f);
	VehicleWheel->SteeringCurve.GetRichCurve()->AddKey(120.f, 0.6f);

	VehicleWheel->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_4W;

	VehicleWheel->DifferentialSetup.FrontRearSplit = 0.65f;

	VehicleWheel->TransmissionSetup.bUseGearAutoBox = true;
	VehicleWheel->TransmissionSetup.GearSwitchTime = 0.15;
	VehicleWheel->TransmissionSetup.GearAutoBoxLatency = 1.f;

	UPrimitiveComponent* UpdatedPrimitive = Cast<UPrimitiveComponent>(VehicleWheel->UpdatedComponent);
	if (UpdatedPrimitive) {
		UpdatedPrimitive->BodyInstance.COMNudge = FVector(8.f, 0.f, 0.f);
	}
	VehicleWheel->InertiaTensorScale = FVector(1.f, 1.333f, 1.2f);

	m_SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm Component");
	m_SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 34.f));
	m_SpringArm->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f));
	m_SpringArm->TargetArmLength = 125.f;
	m_SpringArm->bEnableCameraLag = false;
	m_SpringArm->bEnableCameraRotationLag = false;
	m_SpringArm->bInheritPitch = true;
	m_SpringArm->bInheritRoll = true;
	m_SpringArm->bInheritYaw = true;
	m_SpringArm->SetupAttachment(RootComponent);

	m_Camera = CreateDefaultSubobject<UCameraComponent>("Camera Component");
	m_Camera->SetRelativeLocation(FVector(-125.f, 0.f, 0.f));
	m_Camera->SetRelativeRotation(FRotator(10.f, 0.f, 0.f));
	m_Camera->FieldOfView = 90.f;
	m_Camera->bUsePawnControlRotation = false;
	m_Camera->SetupAttachment(m_SpringArm, USpringArmComponent::SocketName);

	m_bIsPlayer = false;
	PrimaryActorTick.bCanEverTick = true;
}

void ADefaultVehicleCharacter::BeginPlay() {
	Super::BeginPlay();
	m_bIsPlayer = true;
	if (GetVehicleMovementComponent()) {
		GetVehicleMovementComponent()->bDeprecatedSpringOffsetMode = true;
	}
}

void ADefaultVehicleCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (m_bIsDisconnect) {
		Destroy();
	}

	if (!m_bIsPlayer && GetVehicleMovementComponent()) {
/*		GetVehicleMovementComponent()->SetThrottleInput(m_VehicleState.m_Throttle);
		GetVehicleMovementComponent()->SetSteeringInput(m_VehicleState.m_Steering);
		GetVehicleMovementComponent()->SetHandbrakeInput(m_VehicleState.m_HandBreak);*/
	}
	else if (m_Controller && m_bIsPlayer && GetVehicleMovementComponent()) {
/*		FInputMotionData ControllerData = m_Controller->GetControllerData();
		GetVehicleMovementComponent()->SetThrottleInput(ControllerData.m_Throttle);
		GetVehicleMovementComponent()->SetSteeringInput(ControllerData.m_Steering);
		GetVehicleMovementComponent()->SetHandbrakeInput(ControllerData.m_HandBreak);*/
	}
}

void ADefaultVehicleCharacter::PossessedBy(AController* NewController) {
	m_Controller = Cast<AHANSEIRacingController>(NewController);
	if (!m_Controller) {
		m_Controller = Cast<AHANSEIRacingController>(NewController);
	}
}

void ADefaultVehicleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Forward", this, &ADefaultVehicleCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &ADefaultVehicleCharacter::MoveRight);

	PlayerInputComponent->BindAction("HandBreak", IE_Pressed, this, &ADefaultVehicleCharacter::PressedHandBreak);
	PlayerInputComponent->BindAction("HandBreak", IE_Released, this, &ADefaultVehicleCharacter::ReleaseHandBreak);
}

void ADefaultVehicleCharacter::MoveForward(float Value) {
	GetVehicleMovementComponent()->SetThrottleInput(Value);
	//m_VehicleState.m_Throttle = Value;
}

void ADefaultVehicleCharacter::MoveRight(float Value) {
	GetVehicleMovementComponent()->SetSteeringInput(Value);
//	m_VehicleState.m_Steering = Value;
}

void ADefaultVehicleCharacter::PressedHandBreak() {
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
//	m_VehicleState.m_HandBreak = true;
}

void ADefaultVehicleCharacter::ReleaseHandBreak() {
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
//	m_VehicleState.m_HandBreak = false;
}