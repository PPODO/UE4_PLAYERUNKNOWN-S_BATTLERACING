#include "RedZoneSpawner.h"
#include "BombProjectile.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ConstructorHelpers.h"

ARedZoneSpawner::ARedZoneSpawner() : m_bActivateRedZone(false) {
	ConstructorHelpers::FObjectFinder<UDataTable> CoordinateInformation(L"DataTable'/Game/BluePrint/DataTable/RedZoneData.RedZoneData'");
	if (CoordinateInformation.Succeeded()) {
		CoordinateInformation.Object->GetAllRows<FCoordinateInformation>(L"", m_CoordInformation);
	}

	m_BombProjectileClass = ABombProjectile::StaticClass();

	PrimaryActorTick.bCanEverTick = false;
}

void ARedZoneSpawner::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (m_bActivateRedZone) {
		SpawnProjectileCallbackFunction();
		m_bActivateRedZone = false;
	}
}

void ARedZoneSpawner::SpawnProjectileCallbackFunction() {
	for (auto It : m_CoordInformation) {
		if (It) {
			SpawnBombProjectile(*It);
		}
	}
}