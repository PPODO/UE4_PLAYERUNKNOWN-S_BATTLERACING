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
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

private:
	UFUNCTION()
		void MoveForward(float Value);
	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION()
		void PressedBraking();
	UFUNCTION()
		void ReleaseBraking();

private:
	UPROPERTY()
		class USpringArmComponent* m_SpringArm;
	UPROPERTY()
		class UCameraComponent* m_Camera;
	UPROPERTY()
		class UTextRenderComponent* m_PlayerName;
	UPROPERTY()
		class UAudioComponent* m_EngineSoundComponent;
	UPROPERTY()
		class UMaterial* m_DecalMaterial;

private:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		float m_Health;

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
	bool m_bIsDead;

private:
	void SpawnTireDecal();

private:
	FORCEINLINE void IsDrifting(const FName& SocketName, const float& Angle);

public:
	FORCEINLINE void SetIsItPlayer(bool b) { m_bIsPlayer = b; }
	FORCEINLINE void SetIsDisconnect(bool b) { m_bIsDisconnect = b; }
	FORCEINLINE void SetVehicleState(const struct FInputMotionData& State) { m_VehicleState = State; }
	FORCEINLINE void SetPlayerRank(const int32& NewRank) { m_PlayerCurrentRank = NewRank; }
	FORCEINLINE void SetIsDead(const bool& b) { m_bIsDead = b; }

public:
	void SetMaterialFromUniqueKey(const int32& Index);
	void SetPlayerName(const FString& Text);

public:
	FORCEINLINE bool GetIsPlayer() const { return m_bIsPlayer; }
	FORCEINLINE float GetCurrentHealth() const { return m_Health; }

public:
	UFUNCTION(BlueprintCallable)
		FORCEINLINE FString GetPlayerNickName() const { return m_PlayerNickName; }
	UFUNCTION(BlueprintCallable)
		FORCEINLINE int32 GetPlayerCurrentRank() const { return m_PlayerCurrentRank; }

};
