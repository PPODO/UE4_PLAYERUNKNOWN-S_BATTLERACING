#include "ItemSpawner.h"
#include "ItemBox.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Engine/World.h"

AItemSpawner::AItemSpawner() {
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(L"StaticMesh'/Engine/BasicShapes/Cube.Cube'");

	if (MeshObject.Succeeded()) {
		m_BoardMesh = CreateDefaultSubobject<UStaticMeshComponent>("Item Board Mesh");
		if (m_BoardMesh) {
			m_BoardMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
			m_BoardMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
			m_BoardMesh->SetWorldScale3D(FVector(1.f));
			m_BoardMesh->SetStaticMesh(MeshObject.Object);
			m_BoardMesh->SetupAttachment(RootComponent);
		}
	}

	PrimaryActorTick.bCanEverTick = false;
}

void AItemSpawner::BeginPlay() {
	Super::BeginPlay();

	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FVector DefaultSpawnLocation = GetActorLocation();
	float XDis = ((-GetActorScale().X) / m_MaxItemSpawnCount) * 100;
	for (int32 i = 0; i < m_MaxItemSpawnCount; i++) {
		FVector Cal = (GetActorForwardVector() * (fabsf(XDis) * i)) + GetActorForwardVector() * fabs(XDis / 2);
		GetWorld()->SpawnActor<AItemBox>(DefaultSpawnLocation + (GetActorForwardVector() * (GetActorScale().X * 100) / -2) + FVector(Cal.X, Cal.Y, 100.f), FRotator(0.f), Param);
	}
}