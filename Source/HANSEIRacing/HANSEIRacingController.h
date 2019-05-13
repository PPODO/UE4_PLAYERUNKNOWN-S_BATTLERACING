#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SocketComponent.h"
#include "HANSEIRacingController.generated.h"

UCLASS()
class HANSEIRACING_API AHANSEIRacingController : public APlayerController
{
	GENERATED_BODY()

public:
	AHANSEIRacingController();

protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:
	virtual void Possess(class APawn* Pawn);


public:
	void StartLocationSendTimer();
	void StopLocationSendTimer();

private:
	UFUNCTION()
		void SendActorInformation();

private:
	void RecvControllerData(const FArrayReaderPtr& Ptr, const FIPv4Endpoint& EndPoint);

private:
	class FUDPSocketComponent* m_ControllerConnector;
	struct FInputMotionData m_ControllerData;

private:
	class ADefaultVehicleCharacter* m_Character;
	class AInGameMode* m_GameMode;

private:
	FTimerHandle m_TimerHandle;
	int32 m_CurrentLap;
	int32 m_CurrentSplinePoint;
	float m_SplinePointDistance;

public:
	UFUNCTION(BlueprintCallable)
		void ProcessingOverlapEvent(const int32& NumOfSplinePoint);
	UFUNCTION(BlueprintCallable)
		int32 CalculateSplineDistanceFromSplineComponent(class USplineComponent* SplineClass);

public:
	FORCEINLINE struct FInputMotionData GetControllerData() const { return m_ControllerData; }
	FORCEINLINE int32 GetCurrentSplinePoint() const { return m_CurrentSplinePoint; }
	FORCEINLINE float GetSplinePointDistance() const { return m_SplinePointDistance; }

public:
	UFUNCTION(BlueprintCallable)
		FORCEINLINE int32 GetCurrentLap() const { return m_CurrentLap; }

};