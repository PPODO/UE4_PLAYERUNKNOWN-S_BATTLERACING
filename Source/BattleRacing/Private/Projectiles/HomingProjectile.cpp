#include "Projectiles/HomingProjectile.h"
#include "Projectiles/Rocket.h"
#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ChildActorComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AHomingProjectile::AHomingProjectile() {
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
	mProjectileMovementComponent->HomingAccelerationMagnitude = 2000.f;
	mProjectileMovementComponent->bIsHomingProjectile = true;
	mProjectileMovementComponent->SetIsReplicated(true);

	InitialLifeSpan = 15.f;
	bReplicates = true;
	bReplicateMovement = true;
	PrimaryActorTick.bCanEverTick = false;
}

void AHomingProjectile::BeginPlay() {
	Super::BeginPlay();

	UE_LOG(LogTemp, Error, L"%f", GetLifeSpan());
}

void AHomingProjectile::SetHomingTarget(AActor* target) {
	if (IsValid(target)) {
		mProjectileMovementComponent->HomingTargetComponent = target->GetRootComponent();
	}
}