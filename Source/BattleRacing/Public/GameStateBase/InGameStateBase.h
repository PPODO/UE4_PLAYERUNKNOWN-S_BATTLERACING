#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Includes/DefineStructure.h"
#include "InGameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewPlayerEntered, const FLobbyUserInformation&, newUserInformation);

UCLASS()
class BATTLERACING_API AInGameStateBase : public AGameStateBase {
	GENERATED_BODY()
public:
	AInGameStateBase();

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override final;

public:
	void OnNewPlayerEntered(int32 playerID, const FString& nickname);

protected:
	UFUNCTION()
		void OnRep_UserInformation();

public:
	FOnNewPlayerEntered mOnNewPlayerEntered;

private:
	UPROPERTY(replicatedUsing = OnRep_UserInformation, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		TArray<FLobbyUserInformation> mUsersInformation;

};