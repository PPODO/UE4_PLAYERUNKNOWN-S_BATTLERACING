#include "ItemBox.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"

AItemBox::AItemBox() {
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(L"StaticMesh'/Engine/BasicShapes/Cube.Cube'");

	if (MeshObject.Succeeded()) {
		m_ItemBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>("Item Box Mesh Component");
		if (m_ItemBoxMesh) {
			m_ItemBoxMesh->SetRelativeLocation(FVector(0.f));
			m_ItemBoxMesh->SetRelativeRotation(FRotator(0.f));
			m_ItemBoxMesh->SetWorldScale3D(FVector(0.5f));
			m_ItemBoxMesh->SetStaticMesh(MeshObject.Object);
			m_ItemBoxMesh->SetupAttachment(RootComponent);
		}
	}

	PrimaryActorTick.bCanEverTick = true;
}

void AItemBox::BeginPlay() {
	Super::BeginPlay();

}

void AItemBox::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	
}