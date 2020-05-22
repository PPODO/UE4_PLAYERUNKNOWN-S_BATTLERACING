#include "Items/ItemBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

AItemBox::AItemBox() {
	::ConstructorHelpers::FObjectFinder<UStaticMesh> Mesh(L"StaticMesh'/Game/Meshes/ItemBox/ItemBox.ItemBox'");
	::ConstructorHelpers::FObjectFinder<UMaterial> Material(L"Material'/Game/Materials/ItemBox/Material.Material'");

	if (Mesh.Succeeded() && Material.Succeeded()) {
		mItemBoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(L"Box Collision Component");
		mItemBoxCollisionComponent->SetBoxExtent(FVector(27.5f));
		mItemBoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AItemBox::OnComponentOverlappedBegin);
		RootComponent = mItemBoxCollisionComponent;

		mItemBoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(L"Box Mesh Component");
		mItemBoxMesh->SetRelativeLocation(FVector(6.f, 27.f, 0.f));
		mItemBoxMesh->SetWorldScale3D(FVector(1.5f));
		mItemBoxMesh->SetStaticMesh(Mesh.Object);
		mItemBoxMesh->SetMaterial(0, Material.Object);
		mItemBoxMesh->BodyInstance.SetCollisionProfileName(L"OverlapAll");
		mItemBoxMesh->SetupAttachment(RootComponent);
	}

	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AItemBox::BeginPlay() {
	Super::BeginPlay();

	mRotateDirection = FRotator(FMath::RandRange(-1.f, 1.f), FMath::RandRange(-1.f, 1.f), FMath::RandRange(-1.f, 1.f));
}

void AItemBox::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	AddActorWorldRotation(mRotateDirection);
}

void AItemBox::OnComponentOverlappedBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor != this && HasAuthority()) {
		SetupRespawnTimer();
		UpdateItemBoxState(false);
	}	
}

void AItemBox::SetupRespawnTimer() {
	FTimerDelegate mTimerDelegate;
	mTimerDelegate.BindUObject(this, &AItemBox::UpdateItemBoxState, true);
	GetWorld()->GetTimerManager().SetTimer(mTimerHandle, mTimerDelegate, 5.f, false);
}

void AItemBox::UpdateItemBoxState_Implementation(bool bIsActive) {
	if (bIsActive) {
		mRotateDirection = FRotator(FMath::RandRange(-1.f, 1.f), FMath::RandRange(-1.f, 1.f), FMath::RandRange(-1.f, 1.f));
	}
	SetActorHiddenInGame(!bIsActive);
	SetActorEnableCollision(bIsActive);
}