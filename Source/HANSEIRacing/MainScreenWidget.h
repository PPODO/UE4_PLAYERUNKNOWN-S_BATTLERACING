#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SocketComponent.h"
#include <sstream>
#include "MainScreenWidget.generated.h"

UCLASS()
class HANSEIRACING_API UMainScreenWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable)
		TArray<UUserWidget*> CreateSessionLists();

public:
	UPROPERTY(BlueprintReadOnly)
		int32 m_JoinSessionFailReason;
	UPROPERTY(BlueprintReadWrite)
		int32 m_CreateSessionFailedReason;
	UPROPERTY(BlueprintReadWrite)
		int32 m_MaxSessionCount;
	UPROPERTY(BlueprintReadWrite)
		int32 m_CurrentPage;
	UPROPERTY(BlueprintReadWrite)
		bool m_bCreateSessionFailed;
	UPROPERTY(BlueprintReadWrite)
		bool m_bJoinSessionFailed;
	UPROPERTY(BlueprintReadWrite)
		bool m_bResetList;
	UPROPERTY(BlueprintReadWrite)
		TSubclassOf<UUserWidget> m_ListWidgetClass;

public:
	void SetSessionInformation(std::stringstream& RecvStream);
	bool SucceedJoinSession(std::stringstream& RecvStream);
	void FailedCreateSession(const bool& bFailed, const int32& FailedReason);

private:
	SESSION::SessionInformation m_SessionInformations;

};