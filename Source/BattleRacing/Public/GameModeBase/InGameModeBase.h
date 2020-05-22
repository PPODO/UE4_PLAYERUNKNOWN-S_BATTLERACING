#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Includes/DefineEnums.h"
#include "InGameModeBase.generated.h"

UCLASS()
class BATTLERACING_API AInGameModeBase : public AGameModeBase {
	GENERATED_BODY()
public:
	explicit AInGameModeBase();

protected:
	virtual void PostLogin(class APlayerController* playerController) override final;
	virtual void Logout(class AController* exiting) override final;

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		int32 mMaxNumberOfPeole = 3;
	
public:
	TMultiMap<ETeamType, class AVehiclePlayerController*> mPlayerControllers;

};