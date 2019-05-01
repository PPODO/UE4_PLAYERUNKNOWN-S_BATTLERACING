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
	UPROPERTY()
		class UTextRenderComponent* m_PlayerName;
	UPROPERTY()
		class UAudioComponent* m_EngineSoundComponent;

private:
	TArray<class UMaterialInstance*> m_MaterialInstances;
	TArray<const TCHAR*> m_ReferenceSrcs;

private:
	class AInGameMode* m_GameMode;
	class AHANSEIRacingController* m_Controller;
	struct FInputMotionData m_VehicleState;

private:
	int32 m_PlayerCurrentRank;
	FString m_PlayerNickName;
	bool m_bIsDisconnect;
	bool m_bIsPlayer;

public:
	FORCEINLINE void SetIsItPlayer(bool b) { m_bIsPlayer = b; }
	FORCEINLINE void SetIsDisconnect(bool b) { m_bIsDisconnect = b; }
	FORCEINLINE void SetVehicleState(const struct FInputMotionData& State) { m_VehicleState = State; }
	FORCEINLINE void SetPlayerRank(const int32& NewRank) { m_PlayerCurrentRank = NewRank; }

public:
	void SetMaterialFromUniqueKey(const int32& Index);
	void SetPlayerName(const FString& Text);

public:
	UFUNCTION(BlueprintCallable)
		FORCEINLINE FString GetPlayerNickName() const { return m_PlayerNickName; }
	UFUNCTION(BlueprintCallable)
		FORCEINLINE int32 GetPlayerCurrentRank() const { return m_PlayerCurrentRank; }

};