#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "VehiclePawn.generated.h"

UCLASS()
class TESTVEHICLE_API AVehiclePawn : public AWheeledVehicle
{
	GENERATED_BODY()

public:
	AVehiclePawn();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
private:
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* m_SpringArmComp;
	UPROPERTY(VisibleDefaultsOnly)
		class UCameraComponent* m_CameraComp;

private:
	UFUNCTION()
		void MoveForward(float Value);
	UFUNCTION()
		void MoveRight(float Value);
	UFUNCTION()
		void PressedHandBreak();
	UFUNCTION()
		void ReleaseHandBreak();

};
