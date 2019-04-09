#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HANSEIRacingGameInstance.generated.h"

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
	int32 m_SessionID;
	FString m_PlayerNickName;
	FString m_SessionName;
	bool m_bIsLogined;
	int32 m_UniqueKey;

public:
	FORCEINLINE void SetPlayerNickName(const FString& NickName) { m_PlayerNickName = NickName; }
	FORCEINLINE void SetSessionName(const FString& Name) { m_SessionName = Name; }
	FORCEINLINE void SetIsLogined(const bool b) { m_bIsLogined = b; }
	FORCEINLINE void SetUniqueKey(int32 Key) { m_UniqueKey = Key; }
	FORCEINLINE void SetSessionID(const int32 ID) { m_SessionID = ID; }

public:
	FORCEINLINE FString GetSessionName() const { return m_SessionName; }
	FORCEINLINE int32 GetUniqueKey() const { return m_UniqueKey; }
	FORCEINLINE int32 GetSessionID() const { return m_SessionID; }

public:
	UFUNCTION(BlueprintCallable)
		FORCEINLINE FString GetPlayerNickName() const { return m_PlayerNickName; }
	UFUNCTION(BlueprintCallable)
		FORCEINLINE bool GetIsLogined() const { return m_bIsLogined; }

};