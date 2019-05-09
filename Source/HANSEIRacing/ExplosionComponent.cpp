#include "ExplosionComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructorHelpers.h"

UExplosionComponent::UExplosionComponent() {
	ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionParticleObject(L"ParticleSystem'/Game/Particle/ProjectilesPack/Particles/Effects/P_Explosion.P_Explosion'");
	ConstructorHelpers::FObjectFinder<USoundCue> ExplosionSoundObject(L"SoundCue'/Game/Particle/ProjectilesPack/Sounds/Common/Explosion_02_Cue.Explosion_02_Cue'");

	if (ExplosionParticleObject.Succeeded()) {
		m_ExplosionParticle = ExplosionParticleObject.Object;
	}
	if (ExplosionSoundObject.Succeeded()) {
		m_ExplosionSound = ExplosionSoundObject.Object;
	}

	PrimaryComponentTick.bCanEverTick = false;
}

void UExplosionComponent::Excute(const FVector& Location) {
	UGameplayStatics::SpawnEmitterAtLocation(this, m_ExplosionParticle, Location);
	UGameplayStatics::SpawnSoundAtLocation(this, m_ExplosionSound, Location);
}