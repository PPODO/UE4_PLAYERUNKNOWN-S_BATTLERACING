#include "Rocket.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "ConstructorHelpers.h"

ARocket::ARocket() {
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(L"StaticMesh'/Game/Particle/ProjectilesPack/Meshes/Projectiles/Rockets/SM_Rocket_05.SM_Rocket_05'");
	ConstructorHelpers::FObjectFinder<UParticleSystem> TrailParticle(L"ParticleSystem'/Game/Particle/ProjectilesPack/Particles/Effects/P_Smoke_Trail.P_Smoke_Trail'");
	ConstructorHelpers::FObjectFinder<UParticleSystem> FlareParticle(L"ParticleSystem'/Game/Particle/ProjectilesPack/Particles/Effects/P_Flare.P_Flare'");

	m_SceneCollision = CreateDefaultSubobject<USceneComponent>(L"Scene Component");
	if (m_SceneCollision) {
		RootComponent = m_SceneCollision;
	}

	if (MeshObject.Succeeded()) {
		m_RocketMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(L"Rocket Mesh");
		if (m_RocketMeshComp) {
			m_RocketMeshComp->SetCollisionProfileName("NoCollision");
			m_RocketMeshComp->SetStaticMesh(MeshObject.Object);
			m_RocketMeshComp->SetRelativeLocation(FVector(0.f, 0.f, -100.f));
			m_RocketMeshComp->SetRelativeRotation(FRotator(0.f));
			m_RocketMeshComp->SetWorldScale3D(FVector(0.5f));
			m_RocketMeshComp->SetupAttachment(RootComponent);
		}
	}

	if (FlareParticle.Succeeded()) {
		m_FlareParticle = CreateDefaultSubobject<UParticleSystemComponent>(L"Flare Particle Component");
		if (m_FlareParticle) {
			m_FlareParticle->SetTemplate(FlareParticle.Object);
			m_FlareParticle->SetupAttachment(m_RocketMeshComp);
		}
	}

	if (TrailParticle.Succeeded()) {
		m_TrailParticle = CreateDefaultSubobject<UParticleSystemComponent>(L"Trail Particle Component");
		if (m_TrailParticle) {
			m_TrailParticle->SetTemplate(TrailParticle.Object);
			m_TrailParticle->SetWorldScale3D(FVector(0.15f));
			m_TrailParticle->SetupAttachment(m_RocketMeshComp);
		}
	}

	PrimaryActorTick.bCanEverTick = false;
}