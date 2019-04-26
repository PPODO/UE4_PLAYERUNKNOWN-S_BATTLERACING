#include "ItemBox.h"
#include "ItemSpawner.h"
#include "InGameMode.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"

AItemBox::AItemBox() : m_GameMode(nullptr) {
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(L"StaticMesh'/Engine/BasicShapes/Cube.Cube'");

	m_BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>("Item Box Collision Component");
	if (m_BoxCollisionComponent) {
		m_BoxCollisionComponent->SetCollisionProfileName("OverlapAll");
		m_BoxCollisionComponent->SetRelativeLocation(FVector(0.f));
		m_BoxCollisionComponent->SetRelativeRotation(FRotator(0.f));
		m_BoxCollisionComponent->InitBoxExtent(FVector(50.f));
		m_BoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AItemBox::OnComponentOverlapBegin);
		RootComponent = m_BoxCollisionComponent;
	}

	if (MeshObject.Succeeded()) {
		m_ItemBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>("Item Box Mesh Component");
		if (m_ItemBoxMesh) {
			m_ItemBoxMesh->SetupAttachment(m_BoxCollisionComponent);
			m_ItemBoxMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			m_ItemBoxMesh->SetRelativeLocation(FVector(0.f));
			m_ItemBoxMesh->SetRelativeRotation(FRotator(0.f));
			m_ItemBoxMesh->SetStaticMesh(MeshObject.Object);
		}
	}
	PrimaryActorTick.bCanEverTick = true;
}

void AItemBox::BeginPlay() {
	Super::BeginPlay();

	m_RotateDirection = FRotator(1.f, -1.f, 1.f);

	if (!IsValid(m_GameMode)) {
		m_GameMode = Cast<AInGameMode>(GetWorld()->GetAuthGameMode());
		if (!IsValid(m_GameMode)) {
			m_GameMode = Cast<AInGameMode>(GetWorld()->GetAuthGameMode());
		}
	}
}

void AItemBox::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (IsValid(m_BoxCollisionComponent)) {
//		m_BoxCollisionComponent->AddRelativeRotation(FQuat(m_RotateDirection));
	}
}

void AItemBox::OnComponentOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) {
	if (OtherActor && OtherActor != this && OtherComp) {
		if (OtherComp->GetCollisionObjectType() == ECC_Vehicle) {
			if (IsValid(m_BoxCollisionComponent)) {
				if (IsValid(GetOwner()) && IsValid(m_GameMode)) {
					m_GameMode->SendRespawnItemToServer(Cast<AItemSpawner>(GetOwner())->GetSpawnerUniqueID(), m_IndexNumber);
				}
				m_BoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
			SetActorHiddenInGame(true);
		}
	}
}

void AItemBox::ResetItemBox() {
	if (IsValid(m_BoxCollisionComponent)) {
		m_BoxCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SetActorHiddenInGame(false);
	}
}