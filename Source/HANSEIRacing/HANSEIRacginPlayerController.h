#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SocketComponent.h"
#include "HANSEIRacginPlayerController.generated.h"

UCLASS()
class HANSEIRACING_API AHANSEIRacginPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AHANSEIRacginPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

public:
	virtual void Possess(class APawn* Pawn);


public:
	void LocationSendTimerStart(class AGameModeBase* GM);

private:
	UFUNCTION()
		void IsItNearlyEqualLocationAndRotation();

private:
	class ACharacter* m_Character;
	class AInGameModeBase* m_GameMode;

private:
	FTimerHandle m_TimerHandle;
	PLAYER::Vector Location;
	PLAYER::Vector Rotation;

};
