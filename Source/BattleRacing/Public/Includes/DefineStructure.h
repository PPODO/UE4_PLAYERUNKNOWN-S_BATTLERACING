#pragma once

#include "CoreMinimal.h"
#include "Includes/DefineEnums.h"
#include "DefineStructure.generated.h"

USTRUCT(BlueprintType)
struct FLobbyUserInformation {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
		int32 mPlayerID;
	UPROPERTY(BlueprintReadWrite)
		FString mNickname;
	UPROPERTY(BlueprintReadWrite)
		ETeamType mTeam;
	UPROPERTY(BlueprintReadWrite)
		bool mbIsReady;
	UPROPERTY(BlueprintReadWrite)
		bool mbIsRoomLeader;

public:
	FLobbyUserInformation() : mPlayerID(0), mbIsReady(false), mbIsRoomLeader(false), mTeam() {};
	FLobbyUserInformation(int32 playerID, const FString& nickname, ETeamType teamType, bool bIsReady = false, bool IsRoomLeader = false)
		: mPlayerID(playerID), mNickname(nickname), mTeam(teamType), mbIsReady(bIsReady), mbIsRoomLeader(mbIsRoomLeader) {}

};