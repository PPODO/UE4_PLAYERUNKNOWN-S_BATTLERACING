#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginGameMode.h"
#include "LoginWidget.generated.h"

UCLASS()
class HANSEIRACING_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	ALoginGameMode* m_GameMode;

public:
	UPROPERTY(BlueprintReadWrite)
		bool m_bIsPopup = false;

	UPROPERTY(BlueprintReadWrite)
		EPACKETFAILEDTYPE m_FailedReason;

public:
	void SetPopUp(const bool& Value, const EPACKETFAILEDTYPE& FailedReason) { m_bIsPopup = Value, m_FailedReason = FailedReason; }

public:
	UFUNCTION(BlueprintCallable)
		void SetGameMode(AGameModeBase* GM) { m_GameMode = Cast<ALoginGameMode>(GM); }
	
};
