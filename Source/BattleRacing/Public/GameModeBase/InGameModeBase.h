#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Includes/DefineEnums.h"
#include "Includes/DefineStructure.h"
#include "InGameModeBase.generated.h"

UCLASS()
class BATTLERACING_API AInGameModeBase : public AGameModeBase {
	GENERATED_BODY()
public:
	explicit AInGameModeBase();

protected:
	virtual void PostLogin(class APlayerController* playerController) override final;
	virtual void Logout(class AController* exiting) override final;

public:
	FORCEINLINE bool ValidatePlayerID(int32 playerID) { return static_cast<bool>(mPlayerControllers.Find(playerID)); }
	
	ETeamType AllocateTeamType();

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TMap<int32, class AVehiclePlayerController*> mPlayerControllers;

private:
	const int32 mMaxOfPeople{ 3 };
	int32 mCurrentNumberOfRedTeam{ 0 };
	int32 mCurrentNumberOfBlueTeam{ 0 };

};