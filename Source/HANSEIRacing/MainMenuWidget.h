#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginGameMode.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class HANSEIRACING_API UMainMenuWidget : public UUserWidget {
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite)
		bool m_bIsPopup;
	UPROPERTY(BlueprintReadWrite)
		int32 m_CurrentPage;
	UPROPERTY(BlueprintReadOnly)
		int32 m_MaxPage;
	UPROPERTY(BlueprintReadOnly)
		EPACKETFAILEDTYPE m_FailedReason;
	UPROPERTY(BlueprintReadOnly)
		bool m_bIsRefresing;
	UPROPERTY(BlueprintReadWrite)
		TSubclassOf<UUserWidget> m_WidgetClass;

private:
	ALoginGameMode* m_GameMode;
	class SessionInformation m_SessionList;

public:
	void SetPopup(const bool& Value, const EPACKETFAILEDTYPE& Reason) { m_bIsPopup = Value, m_FailedReason = Reason; }
	void SetSessionList(class SessionInformation& List) { m_SessionList = List; m_bIsRefresing = true; };

public:
	UFUNCTION(BlueprintCallable)
		void SetGameMode(AGameModeBase* GM) { m_GameMode = Cast<ALoginGameMode>(GM); }

	UFUNCTION(BlueprintCallable)
		TArray<UUserWidget*> CreateSessionList();
};
