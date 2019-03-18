#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HANSEIRacingGameInstance.generated.h"

UENUM(BlueprintType)
enum EPACKETMESSAGE {
	PM_SIGNUP,
	PM_LOGIN,
	PM_CREATESESSION,
	PM_SESSIONLIST,
	PM_JOINSESSION,
	PM_DISCONNECT
};

UENUM(BlueprintType)
enum EFAILED {
	EF_EXIST,
	EF_FAILED,
	EF_SUCCEED
};

UENUM(BlueprintType)
enum EGAMEPACKETMESSAGE {
	PGM_JOIN,
	PGM_DISCONNECT,
	PGM_UPDATE,

	PGM_DISCONNECTOTHER,
	PGM_NEWPLAYER,
	PGM_COUNT
};

UCLASS()
class HANSEIRACING_API UHANSEIRacingGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UHANSEIRacingGameInstance();

protected:
	virtual void Init() override;
	virtual void Shutdown() override;

private:
	FString m_PlayerNickName;
	FString m_SessionName;
	bool m_bIsLogined;
	int32 m_UniqueKey;

public:
	FORCEINLINE void SetPlayerNickName(const FString& NickName) { m_PlayerNickName = NickName; }
	FORCEINLINE void SetSessionName(const FString& Name) { m_SessionName = Name; }
	FORCEINLINE void SetIsLogined(const bool b) { m_bIsLogined = b; }
	FORCEINLINE void SetUniqueKey(int32 Key) { m_UniqueKey = Key; }
	FORCEINLINE int32 GetUniqueKey() const { return m_UniqueKey; }

public:
	FORCEINLINE FString GetSessionName() const { return m_SessionName; }

public:
	UFUNCTION(BlueprintCallable)
		FORCEINLINE FString GetPlayerNickName() const { return m_PlayerNickName; }
	UFUNCTION(BlueprintCallable)
		FORCEINLINE bool GetIsLogined() const { return m_bIsLogined; }

};