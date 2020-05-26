#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "VehiclePlayerState.generated.h"

UCLASS()
class BATTLERACING_API AVehiclePlayerState : public APlayerState {
	GENERATED_BODY()
public:
	virtual void OnRep_PlayerId() override final;

private:
	UFUNCTION(Server, Reliable, WithValidation)
		void OnInitializationSucceed(int32 playerUniqueID, const FString& nickname);

};