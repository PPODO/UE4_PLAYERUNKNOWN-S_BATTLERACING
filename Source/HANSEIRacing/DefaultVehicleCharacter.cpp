#include "DefaultVehicleCharacter.h"
#include "FrontVehicleWheel.h"
#include "RearVehicleWheel.h"
#include "HANSEIRacingController.h"
#include "InGameMode.h"
#include "WheeledVehicleMovementComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/TextRenderComponent.h"
#include "Components/InputComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "ConstructorHelpers.h"
#include "Sound/SoundCue.h"
#include "Engine/Engine.h"

ADefaultVehicleCharacter::ADefaultVehicleCharacter() : m_Controller(nullptr), m_bIsPlayer(false), m_bIsDisconnect(false), m_GameMode(nullptr) {
	AIControllerClass = APlayerController::StaticClass();

	ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterial(L"Material'/Game/Materials/Decal/DecalMaterial.DecalMaterial'");
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshObject(L"SkeletalMesh'/Game/Vehicle/Vehicle_SkelMesh.Vehicle_SkelMesh'");
	ConstructorHelpers::FObjectFinder<USoundCue> EngineSoundCue(L"SoundCue'/Game/Vehicle/Sound/Engine_Loop_Cue.Engine_Loop_Cue'");
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstance(L"'/Game/Vehicle/VehicleAnimationBlueprint'");

	if (DecalMaterial.Succeeded()) {
		m_DecalMaterial = DecalMaterial.Object;
	}
	
	m_ReferenceSrcs.Add(L"'/Game/Materials/AutomotiveMaterials/Materials/Metal/MI_Metal_Chrome_03_Dark.MI_Metal_Chrome_03_Dark'");
	m_ReferenceSrcs.Add(L"'/Game/Materials/AutomotiveMaterials/Materials/CaliperPaint/MI_Caliper_Silver.MI_Caliper_Silver'");
	m_ReferenceSrcs.Add(L"'/Game/Materials/AutomotiveMaterials/Materials/CarPaint/MI_CarPaint_05_White.MI_CarPaint_05_White'");
	m_ReferenceSrcs.Add(L"'/Game/Materials/AutomotiveMaterials/Materials/CarPaint/MI_CarPaint_10_Red.MI_CarPaint_10_Red'");
	m_ReferenceSrcs.Add(L"'/Game/Materials/AutomotiveMaterials/Materials/Metal/MI_Metal_Chrome_01_Bright.MI_Metal_Chrome_01_Bright'");
	m_ReferenceSrcs.Add(L"'/Game/Materials/AutomotiveMaterials/Materials/ExhaustFinish/M_Exhaust_Stealth.M_Exhaust_Stealth'");
	m_ReferenceSrcs.Add(L"'/Game/Materials/AutomotiveMaterials/Materials/CarPaint/MI_CarPaint_11_Orange.MI_CarPaint_11_Orange'");
	m_ReferenceSrcs.Add(L"'/Game/Materials/AutomotiveMaterials/Materials/CarPaint/MI_CarPaint_12_Yellow.MI_CarPaint_12_Yellow'");
	m_ReferenceSrcs.Add(L"'/Game/Materials/AutomotiveMaterials/Materials/CarPaint/MI_CarPaint_13_Green.MI_CarPaint_13_Green'");

	if (MeshObject.Succeeded() && AnimInstance.Succeeded()) {
		GetMesh()->SetSkeletalMesh(MeshObject.Object);
		GetMesh()->SetAnimInstanceClass(AnimInstance.Class);
	}

	if (EngineSoundCue.Succeeded()) {
		m_EngineSoundComponent = CreateDefaultSubobject<UAudioComponent>("Engine Sound Component");
		if (m_EngineSoundComponent) {
			m_EngineSoundComponent->SetupAttachment(RootComponent);
			m_EngineSoundComponent->SetSound(EngineSoundCue.Object);
		}
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

	VehicleWheel->bDeprecatedSpringOffsetMode = true;
//	VehicleWheel->bDeprecatedSpringOffsetMode = false;
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
	m_SpringArm->bDoCollisionTest = false;
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

	m_PlayerName = CreateDefaultSubobject<UTextRenderComponent>("Player Name");
	m_PlayerName->SetupAttachment(RootComponent);
	m_PlayerName->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	m_PlayerName->SetRelativeLocation(FVector(0.f, 0.f, 75.f));
	m_PlayerName->SetRelativeRotation(FRotator(0.f));
	m_PlayerName->SetHiddenInGame(true);

	m_bIsPlayer = false;
	PrimaryActorTick.bCanEverTick = true;
}

void ADefaultVehicleCharacter::BeginPlay() {
	Super::BeginPlay();

	if (GetMesh()) {
		GetMesh()->SetCenterOfMass(FVector(0.f, 0.f, -30.f), "Vehicle");
	}

	if (IsValid(m_EngineSoundComponent)) {
		m_EngineSoundComponent->Play();
	}

	m_GameMode = Cast<AInGameMode>(GetWorld()->GetAuthGameMode());
	if (!m_GameMode) {
		m_GameMode = Cast<AInGameMode>(GetWorld()->GetAuthGameMode());
	}
}

void ADefaultVehicleCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (IsInGameThread()) {
		if (m_bIsDisconnect) {
			Destroy();
		}

		if (!m_bIsPlayer && GetVehicleMovement()) {
/*			GetVehicleMovement()->SetThrottleInput(m_VehicleState.m_Throttle);
			GetVehicleMovement()->SetSteeringInput(m_VehicleState.m_Steering);
			GetVehicleMovement()->SetHandbrakeInput(m_VehicleState.m_HandBreak);*/
		}
		else if (m_Controller && m_bIsPlayer && GetVehicleMovementComponent()) {
/*			FInputMotionData ControllerData = m_Controller->GetControllerData();
			GetVehicleMovementComponent()->SetThrottleInput(ControllerData.m_Throttle);
			GetVehicleMovementComponent()->SetSteeringInput(ControllerData.m_Steering);
			GetVehicleMovementComponent()->SetHandbrakeInput(ControllerData.m_HandBreak);
*/		}

		SpawnTireDecal();

		if (IsValid(m_PlayerName) && !m_PlayerName->bHiddenInGame) {
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			if (IsValid(PlayerController) && IsValid(PlayerController->GetPawn())) {
				FVector Location = PlayerController->GetPawn()->GetActorLocation();
				m_PlayerName->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Location));
			}
		}

		if (IsValid(m_EngineSoundComponent)) {
			float RPMToAudioScale = 2500.0f / GetVehicleMovement()->GetEngineMaxRotationSpeed();
			m_EngineSoundComponent->SetFloatParameter("RPM", GetVehicleMovement()->GetEngineRotationSpeed() * RPMToAudioScale);
		}
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
}

void ADefaultVehicleCharacter::MoveRight(float Value) {
	GetVehicleMovementComponent()->SetSteeringInput(Value);
}

void ADefaultVehicleCharacter::PressedHandBreak() {
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void ADefaultVehicleCharacter::ReleaseHandBreak() {
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void ADefaultVehicleCharacter::SetPlayerName(const FString& Text) {
	if (IsValid(m_PlayerName)) {
		m_PlayerName->SetText(Text);
		m_PlayerName->SetHiddenInGame(m_bIsPlayer);
	}
	m_PlayerNickName = Text;
}

void ADefaultVehicleCharacter::SpawnTireDecal() {
	IsDrifting("PhysWheel_FR", 22.5f);
	IsDrifting("PhysWheel_FL", 22.5f);
	IsDrifting("PhysWheel_BR", 17.5f);
	IsDrifting("PhysWheel_BL", 17.5f);
}

void ADefaultVehicleCharacter::IsDrifting(const FName & SocketName, const float & Angle) {
	if (IsValid(GetMesh()) && IsValid(m_DecalMaterial)) {
		FVector LinearVelocity = GetMesh()->GetPhysicsLinearVelocityAtPoint(GetMesh()->GetSocketLocation(SocketName), SocketName);
		FTransform SocketTransform = GetMesh()->GetSocketTransform(SocketName);
		FVector InverseDirection = UKismetMathLibrary::InverseTransformDirection(SocketTransform, LinearVelocity);
		InverseDirection.Normalize();

		if (UKismetMathLibrary::InRange_FloatFloat(UKismetMathLibrary::Abs((UKismetMathLibrary::DegAcos(FVector::DotProduct(FVector(0.f, 1.f, 0.f), InverseDirection)) - 90)), Angle, 85.f)) {
			UGameplayStatics::SpawnDecalAtLocation(GetWorld(), m_DecalMaterial, FVector(10.f), SocketTransform.GetLocation() - FVector(0.f, 0.f, 18.f), FRotator(0.f, 0.f, SocketTransform.GetRotation().Z));
		}
	}
}

void ADefaultVehicleCharacter::SetMaterialFromUniqueKey(const int32& Index) {
	if (Index >= 0) {
		auto MaterialObject = LoadObject<UMaterialInstance>(nullptr, m_ReferenceSrcs[Index % m_ReferenceSrcs.Num()]);
		if (GetMesh() && MaterialObject) {
			GetMesh()->SetMaterial(0, UMaterialInstanceDynamic::Create(MaterialObject, nullptr));
		}
	}
}