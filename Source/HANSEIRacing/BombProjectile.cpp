#include "BombProjectile.h"
#include "DefaultVehicleCharacter.h"
#include "ExplosionComponent.h"
#include "Rocket.h"
#include "Components/SphereComponent.h"
#include "Components/ChildActorComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFrameWork/DamageType.h"
#include "ConstructorHelpers.h"

ABombProjectile::ABombProjectile() : m_ProjectileSpeed(1000.f) {
	ConstructorHelpers::FClassFinder<ARocket> ChildActorObject(L"Class'/Script/HANSEIRacing.Rocket'");
	
	m_SphereCollision = CreateDefaultSubobject<USphereComponent>(L"Sphere Collision");
	if (m_SphereCollision) {
		m_SphereCollision->GetBodyInstance()->SetCollisionProfileName("Projectile");
		m_SphereCollision->OnComponentHit.AddDynamic(this, &ABombProjectile::OnComponentHit);
		m_SphereCollision->SetSphereRadius(50.f);
		RootComponent = m_SphereCollision;
	}

	m_RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>(L"Radial Force Component");
	if (m_RadialForceComponent) {
		m_RadialForceComponent->bAutoActivate = false;
		m_RadialForceComponent->Radius = 300.f;
		m_RadialForceComponent->ImpulseStrength = 3000000.f;
		m_RadialForceComponent->ForceStrength = 3000000.f;
		m_RadialForceComponent->Falloff = ERadialImpulseFalloff::RIF_Linear;
		m_RadialForceComponent->DestructibleDamage = 50.f;
		m_RadialForceComponent->SetupAttachment(RootComponent);
	}

	if (ChildActorObject.Succeeded()) {
		m_RocketActorComponent = CreateDefaultSubobject<UChildActorComponent>(L"Rocket Child Actor Component");
		if (m_RocketActorComponent) {
			m_RocketActorComponent->SetChildActorClass(ChildActorObject.Class);
			m_RocketActorComponent->SetRelativeLocation(FVector(50.f, 0.f, 0.f));
			m_RocketActorComponent->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
			m_RocketActorComponent->SetupAttachment(RootComponent);
		}
	}

	m_ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(L"Projectile Movement Component");
	m_ProjectileMovement->InitialSpeed = m_ProjectileSpeed;
	m_ProjectileMovement->MaxSpeed = m_ProjectileSpeed;
	m_ProjectileMovement->ProjectileGravityScale = 0.f;
	m_ProjectileMovement->bRotationFollowsVelocity = true;

	m_ExplosionComponent = CreateDefaultSubobject<UExplosionComponent>(L"Explosion Component");

	PrimaryActorTick.bCanEverTick = false;
}

void ABombProjectile::OnComponentHit(UPrimitiveComponent * HitComponent, AActor* OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit) {
	if (IsValid(m_RadialForceComponent)) {
		m_RadialForceComponent->FireImpulse();
	}
	if (HitComponent && OtherActor && OtherActor != this && OtherComp) {
		if (m_RocketActorComponent && m_RocketActorComponent->GetChildActor()) {
			m_RocketActorComponent->DestroyChildActor();
		}
		if (IsValid(m_ExplosionComponent)) {
			m_ExplosionComponent->Excute(GetActorLocation());
		}
		UGameplayStatics::ApplyRadialDamage(GetWorld(), 50.f, GetActorLocation(), m_RadialForceComponent->Radius * 2, UDamageType::StaticClass(), TArray<AActor*>(), this);
		Destroy();
	}
}