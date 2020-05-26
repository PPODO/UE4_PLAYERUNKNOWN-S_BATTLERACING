#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Includes/DefineEnums.h"
#include "VehiclePlayerController.generated.h"

UCLASS()
class BATTLERACING_API AVehiclePlayerController : public APlayerController {
	GENERATED_BODY()
public:
	AVehiclePlayerController();

protected:
	virtual void BeginPlay() override final;
	virtual void Tick(float deltaTime) override final;

public:
	FORCEINLINE void SetTeamType(const ETeamType& type) { mTeamType = type; }
	FORCEINLINE ETeamType GetTeamType() { return mTeamType; }

	FORCEINLINE bool IsCurrentPlayer(int32 playerID) { if (GetPlayerID() == playerID) return true; return false; };
	int32 GetPlayerID();

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		ETeamType mTeamType;

};