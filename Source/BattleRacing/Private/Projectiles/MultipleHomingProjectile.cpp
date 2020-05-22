#include "Projectiles/MultipleHomingProjectile.h"
#include "Projectiles/Rocket.h"
#include "Projectiles/HomingProjectile.h"
#include "Vehicle/WheeledVehicle4W.h"
#include "PlayerController/VehiclePlayerController.h"
#include "GameModeBase/InGameModeBase.h"
#include "Components/ChildActorComponent.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

AMultipleHomingProjectile::AMultipleHomingProjectile() {
	mSceneComponent = CreateDefaultSubobject<USceneComponent>(L"Default Component");
	RootComponent = mSceneComponent;

	mCollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(L"Collision Component");
	mCollisionComponent->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	mCollisionComponent->SetCapsuleHalfHeight(27.5f);
	mCollisionComponent->SetCapsuleRadius(10.f);
	mCollisionComponent->SetupAttachment(RootComponent);

	mChildActorComponent = CreateDefaultSubobject<UChildActorComponent>(L"Child Actor Component");
	mChildActorComponent->SetRelativeLocation(FVector(0.f, 0.f, -25.f));
	mChildActorComponent->SetChildActorClass(ARocket::StaticClass());
	mChildActorComponent->SetupAttachment(mCollisionComponent);

	mProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(L"Projectile Movement Component");
	mProjectileMovementComponent->InitialSpeed = 1000.f;
	mProjectileMovementComponent->MaxSpeed = 1000.f;
	mProjectileMovementComponent->ProjectileGravityScale = 0.f;
	mProjectileMovementComponent->bRotationFollowsVelocity = true;
	mProjectileMovementComponent->bInitialVelocityInLocalSpace = true;
	mProjectileMovementComponent->bInterpMovement = true;
	mProjectileMovementComponent->bInterpRotation = true;
	mProjectileMovementComponent->SetIsReplicated(true);

	bReplicates = true;
	bReplicateMovement = true;
	PrimaryActorTick.bCanEverTick = false;
}

void AMultipleHomingProjectile::BeginPlay() {
	Super::BeginPlay();

	if (HasAuthority()) {
		GetWorld()->GetTimerManager().SetTimer(mSpawnChildProjectileHandler, this, &AMultipleHomingProjectile::SpawnChildProjectiles, 5.f, false);
	}
}

void AMultipleHomingProjectile::SpawnChildProjectiles() {
	auto targets = GetOwner();

	for (int32 i = 0; i < mMaxChildRocket; i++) {
		FTransform newTransform = MakeProjectileTransform(i + 1, UKismetMathLibrary::MakeTransform(GetActorLocation(), mSceneComponent->GetComponentRotation(), FVector(1.f)));
		SpawnProjectilesOnClients(newTransform, targets);
	}
	GetWorld()->GetTimerManager().ClearTimer(mSpawnChildProjectileHandler);
	Destroy();
}

FTransform AMultipleHomingProjectile::MakeProjectileTransform(int32 index, const FTransform& transform) {
	float rotateShift = FMath::DegreesToRadians((static_cast<float>(360.f) / static_cast<float>(mMaxChildRocket) * index));

	FVector newLocation(0.f, FMath::Cos(rotateShift) * mDistanceByProjectile, FMath::Sin(rotateShift) * mDistanceByProjectile);
	FRotator newRotation = FRotator(0.f, GetActorRotation().Yaw, 0.f) + newLocation.Rotation();

	return FTransform(newRotation, transform.GetLocation() + newLocation, transform.GetScale3D());
}

void AMultipleHomingProjectile::SpawnProjectilesOnClients_Implementation(const FTransform& transform, AActor* target) {
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto spawnedActor = GetWorld()->SpawnActor<AHomingProjectile>(AHomingProjectile::StaticClass(), transform, Params);
	if (HasAuthority() && spawnedActor) {
		spawnedActor->SetHomingTarget(target);
	}
}