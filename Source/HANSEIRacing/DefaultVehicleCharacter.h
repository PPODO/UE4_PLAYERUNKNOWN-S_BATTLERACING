#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "DefaultVehicleCharacter.generated.h"

UCLASS()
class HANSEIRACING_API ADefaultVehicleCharacter : public AWheeledVehicle
{
	GENERATED_BODY()

public:
	ADefaultVehicleCharacter();

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
	UPROPERTY()
		class USpringArmComponent* m_SpringArm;
	UPROPERTY()
		class UCameraComponent* m_Camera;

};