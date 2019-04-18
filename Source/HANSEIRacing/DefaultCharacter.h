#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DefaultCharacter.generated.h"

UCLASS()
class HANSEIRACING_API ADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADefaultCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(class AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
		void MoveForward(float Value);
	UFUNCTION()
		void MoveRight(float Value);
	UFUNCTION()
		void TurnRate(float Value);
	UFUNCTION()
		void LookAtRate(float Value);

private:
	UPROPERTY(EditAnywhere)
		class USpringArmComponent* m_SpringArm;
	UPROPERTY(EditAnywhere)
		class UCameraComponent* m_Camera;
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* m_Mesh;

private:
	class AHANSEIRacingController* m_Controller;

public:


};
