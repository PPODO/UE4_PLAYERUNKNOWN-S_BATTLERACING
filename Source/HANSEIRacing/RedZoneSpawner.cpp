#include "RedZoneSpawner.h"
#include "BombProjectile.h"
#include "Engine/World.h"
#include "TimerManager.h"

ARedZoneSpawner::ARedZoneSpawner() {
	m_BombProjectileClass = ABombProjectile::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
}

void ARedZoneSpawner::BeginPlay() {
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(m_SpawnProjectileTimerHandle, this, &ARedZoneSpawner::SpawnProjectileCallbackFunction, 1.f, true, 0.f);
}

void ARedZoneSpawner::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ARedZoneSpawner::BeginDestroy() {
	Super::BeginDestroy();

	if (GetWorld() && m_SpawnProjectileTimerHandle.IsValid()) {
		GetWorld()->GetTimerManager().ClearTimer(m_SpawnProjectileTimerHandle);
	}
}

void ARedZoneSpawner::SpawnProjectileCallbackFunction() {
	int32 SpawnCount = FMath::RandRange(2, 5);
	for (int32 i = 0; i < SpawnCount; i++) {
		FActorSpawnParameters Param;
		Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<ABombProjectile>(m_BombProjectileClass, FVector(FMath::FRandRange(-200.f, 200.f), FMath::FRandRange(-200.f, 200.f), 1000.f), FRotator(-90.f, 0.f, 0.f), Param);
	}
}