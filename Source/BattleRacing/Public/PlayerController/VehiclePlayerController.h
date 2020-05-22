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

public:
	FORCEINLINE void SetTeamType(const ETeamType& type) { mTeamType = type; }
	FORCEINLINE ETeamType GetTeamType() { return mTeamType; }

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		ETeamType mTeamType;

};