#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "SocketComponent.h"
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
	virtual void PossessedBy(class AController* NewController) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
		void MoveForward(float Value);
	UFUNCTION()
		void MoveRight(float Value);
	UFUNCTION()
		void PressedHandBreak();
	UFUNCTION()
		void ReleaseHandBreak();

private:
	UPROPERTY()
		class USpringArmComponent* m_SpringArm;
	UPROPERTY()
		class UCameraComponent* m_Camera;

private:
	class AHANSEIRacingController* m_Controller;

private:
	bool m_bIsDisconnect;
	bool m_bIsPlayer;

private:
//	PLAYER::VehicleState m_VehicleState;

public:
	FORCEINLINE void SetIsItPlayer(bool b) { m_bIsPlayer = b; }
	FORCEINLINE void SetIsDisconnect(bool b) { m_bIsDisconnect = b; }
	//FORCEINLINE void SetVehicleState(PLAYER::VehicleState& State) { m_VehicleState = State; }

public:
//	FORCEINLINE PLAYER::VehicleState GetVehicleState() const { return m_VehicleState; }

};