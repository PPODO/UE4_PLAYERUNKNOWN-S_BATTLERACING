#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DefaultCharacter.generated.h"

UCLASS()
class HANSEIRACING_API ADefaultCharacter : public APawn
{
	GENERATED_BODY()

public:
	ADefaultCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
		void MoveForward(float Value);
	UFUNCTION()
		void MoveRight(float Value);

private:
	class UStaticMeshComponent* m_PawnMesh;

};