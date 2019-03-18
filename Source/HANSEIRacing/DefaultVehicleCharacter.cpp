#include "DefaultVehicleCharacter.h"
#include "FrontVehicleWheel.h"
#include "RearVehicleWheel.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "ConstructorHelpers.h"

ADefaultVehicleCharacter::ADefaultVehicleCharacter() {
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObject(L"SkeletalMesh'/Game/Vehicle/Vehicle_SkelMesh.Vehicle_SkelMesh'");

	if (MeshObject.Succeeded()) {
		GetMesh()->SetSkeletalMesh(MeshObject.Object);

	}

/*	UWheeledVehicleMovementComponent4W* VehicleWheel = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());

	check(VehicleWheel->Wheels.Num() == 4)

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

	VehicleWheel->InertiaTensorScale = FVector(1.f, 1.333f, 1.2f);*/

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
}

void ADefaultVehicleCharacter::BeginPlay() {
	Super::BeginPlay();

}

void ADefaultVehicleCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void ADefaultVehicleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);


}

void ADefaultVehicleCharacter::MoveForward(float Value) {
}

void ADefaultVehicleCharacter::MoveRight(float Value) {
}
