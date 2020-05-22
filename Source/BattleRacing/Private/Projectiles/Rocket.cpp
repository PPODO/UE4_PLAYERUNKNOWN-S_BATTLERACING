#include "Projectiles/Rocket.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"

ARocket::ARocket() {
	::ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(L"StaticMesh'/Game/ProjectilesPack/Meshes/Projectiles/Rockets/SM_Rocket_02.SM_Rocket_02'");
	::ConstructorHelpers::FObjectFinder<UParticleSystem> FlareParticle(L"ParticleSystem'/Game/ProjectilesPack/Particles/Effects/P_Flare.P_Flare'");
	::ConstructorHelpers::FObjectFinder<UParticleSystem> SmokeParticle(L"ParticleSystem'/Game/ProjectilesPack/Particles/Effects/P_Smoke_Trail.P_Smoke_Trail'");

	mRocketMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(L"Rocket Mesh Component");
	RootComponent = mRocketMeshComponent;
	if (MeshObject.Succeeded()) {
		mRocketMeshComponent->SetStaticMesh(MeshObject.Object);
	}

	mFlareParticle = CreateDefaultSubobject<UParticleSystemComponent>(L"Flare Component");
	mFlareParticle->SetWorldScale3D(FVector(0.5f));
	mFlareParticle->SetupAttachment(RootComponent);
	if (FlareParticle.Succeeded()) {
		mFlareParticle->SetTemplate(FlareParticle.Object);
	}

	mSmokeParticle = CreateDefaultSubobject<UParticleSystemComponent>(L"Smoke Component");
	mSmokeParticle->SetWorldScale3D(FVector(0.125f));
	mSmokeParticle->SetupAttachment(RootComponent);
	if (SmokeParticle.Succeeded()) {
		mSmokeParticle->SetTemplate(SmokeParticle.Object);
	}

	PrimaryActorTick.bCanEverTick = false;
}