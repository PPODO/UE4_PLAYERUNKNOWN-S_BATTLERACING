#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameWidget.generated.h"

UCLASS()
class HANSEIRACING_API UInGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		TArray<class ADefaultVehicleCharacter*> m_CharacterClass;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		int m_Count = 0;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		FString m_Message;
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		bool m_bIsDisableTimer = true;

private:
	struct FTimerHandle m_CountdownTimerHandle;
	TFunction<void()> m_CountdownFinishedCallback;

	bool m_bIsUpdateClassData = false;

private:
	UFUNCTION()
		void TimerCallbackFunction();

public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdatePlayerList();
		void UpdatePlayerList_Implementation();

public:
	void SetCharacterClassData(const TMap<int32, class ADefaultVehicleCharacter*>& Data);
	void SetCountdownTimer(const int32& Count, const FString& Message, const TFunction<void()>& Callback);

};