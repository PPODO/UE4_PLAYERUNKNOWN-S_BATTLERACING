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
	void LocationSendTimerStart(class AGameModeBase* GM);

private:
	UFUNCTION()
		void IsItNearlyEqualActorInformation();

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

public:
	FORCEINLINE struct FInputMotionData GetControllerData() const { return m_ControllerData; }

};