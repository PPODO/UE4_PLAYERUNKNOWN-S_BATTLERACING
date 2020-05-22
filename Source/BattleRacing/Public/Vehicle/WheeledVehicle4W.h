#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "WheeledVehicle4W.generated.h"

UCLASS()
class BATTLERACING_API AWheeledVehicle4W : public AWheeledVehicle {
	GENERATED_BODY()
public:
	AWheeledVehicle4W();

protected:
	virtual void BeginPlay() override final;
	virtual void Tick(float deltaTime) override final;
	virtual void SetupPlayerInputComponent(UInputComponent* playerInputComponent) override final;

private:
	UFUNCTION()
		void MoveForward(float val);
	UFUNCTION()
		void MoveRight(float val);
	UFUNCTION()
		void PressedBrake();
	UFUNCTION()
		void ReleasedBrake();

private:
	void InitializeVehicle();
	void UpdateVehicleGravity();
	void CalculateEngineSound();

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* mCameraComponent;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* mSpringArmComponent;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		class UAudioComponent* mEngineSoundComponent;
	UPROPERTY(EditAnywhere, Category = "Gravity", meta = (AllowPrivateAccess = "true"))
		float mTraceMaxLength = 500.f;
	UPROPERTY(EditAnywhere, Category = "Gravity", meta = (AllowPrivateAccess = "true"))
		float mGravityScale = -19.62f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		bool mbIsUpdateCustomGravity = true;

};