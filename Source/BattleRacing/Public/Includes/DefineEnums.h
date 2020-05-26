#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefineEnums.generated.h"

UENUM(BlueprintType)
enum class ETeamType : uint8 {
	Team_None,
	Team_Red,
	Team_Blue
};
