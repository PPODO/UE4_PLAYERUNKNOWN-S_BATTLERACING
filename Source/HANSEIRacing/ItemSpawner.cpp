#include "ItemSpawner.h"
#include "ItemBox.h"
#include "InGameMode.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

AItemSpawner::AItemSpawner() {
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(L"StaticMesh'/Engine/BasicShapes/Cube.Cube'");

	if (MeshObject.Succeeded()) {
		m_BoardMesh = CreateDefaultSubobject<UStaticMeshComponent>("Item Board Mesh");
		if (m_BoardMesh) {
			m_BoardMesh->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
			m_BoardMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
			m_BoardMesh->SetWorldScale3D(FVector(1.f));
			m_BoardMesh->SetStaticMesh(MeshObject.Object);
			SetRootComponent(m_BoardMesh);
		}
	}

	PrimaryActorTick.bCanEverTick = false;
}

void AItemSpawner::BeginPlay() {
	Super::BeginPlay();

	if (m_MaxItemSpawnCount < 2) {
		m_MaxItemSpawnCount = 2;
	}

	for (int32 i = 0; i < m_MaxItemSpawnCount; i++) {
		UChildActorComponent* ChildActorComp = NewObject<UChildActorComponent>(this, UChildActorComponent::StaticClass());
		if (IsValid(ChildActorComp)) {
			FVector SpawnLocation = FVector((100.f / (m_MaxItemSpawnCount)) * (i - (m_MaxItemSpawnCount / 2)) + ((25.f / (m_MaxItemSpawnCount / 2)) * (m_MaxItemSpawnCount % 2 == 0 ? 1 : 0)), 0.f, 100.f);
			ChildActorComp->AttachTo(RootComponent);
			ChildActorComp->SetChildActorClass(AItemBox::StaticClass());
			ChildActorComp->SetRelativeLocation(SpawnLocation);
			ChildActorComp->SetRelativeRotation(FRotator(0.f));
			ChildActorComp->SetWorldScale3D(FVector(0.5f));
			ChildActorComp->RegisterComponent();
			m_ItemBoxActors.Add(ChildActorComp);

			AItemBox* Actor = Cast<AItemBox>(ChildActorComp->GetChildActor());
			if (IsValid(Actor)) {
				Actor->SetOwner(this);
				Actor->SetIndexNumber(i);
			}
		}
	}
}

void AItemSpawner::FindItemIndexAndReset(const int32& ItemIndex) {
	if (ItemIndex >= 0 && ItemIndex < m_ItemBoxActors.Num()) {
		auto ItemBoxActor = Cast<AItemBox>(m_ItemBoxActors[ItemIndex]->GetChildActor());
		if (IsValid(ItemBoxActor)) {
			ItemBoxActor->ResetItemBox();
		}
	}
}