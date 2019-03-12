#include "DefaultCharacter.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"

ADefaultCharacter::ADefaultCharacter() {
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(L"StaticMesh'/Engine/BasicShapes/Cube.Cube'");

	if (MeshObject.Succeeded()) {
		m_PawnMesh = CreateDefaultSubobject<UStaticMeshComponent>(L"Pawn Mesh Component");
		m_PawnMesh->SetStaticMesh(MeshObject.Object);
	}

	PrimaryActorTick.bCanEverTick = true;
}

void ADefaultCharacter::BeginPlay() {
	Super::BeginPlay();

}

void ADefaultCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void ADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Forward", this, &ADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &ADefaultCharacter::MoveRight);
}

void ADefaultCharacter::MoveForward(float Value) {

}

void ADefaultCharacter::MoveRight(float Value) {
	
}