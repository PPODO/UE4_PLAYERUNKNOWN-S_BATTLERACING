#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HANSEIRacingGameInstance.generated.h"

UENUM(BlueprintType)
enum EPACKETSTATE {
	PS_EXIST,
	PS_FAILED,
	PS_SUCCESSED
};

UCLASS()
class HANSEIRACING_API UHANSEIRacingGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UHANSEIRacingGameInstance();

private:
	FString m_PlayerNickName;
	bool m_bIsLogined;

public:
	FORCEINLINE void SetPlayerNickName(const FString& NickName) { m_PlayerNickName = NickName; }
	FORCEINLINE void SetIsLogined(const bool b) { m_bIsLogined = b; }

public:
	UFUNCTION(BlueprintCallable)
		FORCEINLINE FString GetPlayerNickName() const { return m_PlayerNickName; }
	UFUNCTION(BlueprintCallable)
		FORCEINLINE bool GetIsLogined() const { return m_bIsLogined; }

};