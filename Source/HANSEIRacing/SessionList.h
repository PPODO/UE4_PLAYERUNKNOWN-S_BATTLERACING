#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionList.generated.h"

UCLASS()
class HANSEIRACING_API USessionList : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
		FString m_SessionName;
	UPROPERTY(BlueprintReadOnly)
		int32 m_MaxPlayer;
	UPROPERTY(BlueprintReadOnly)
		int32 m_CurrentPlayer;
	UPROPERTY(BlueprintReadOnly)
		bool m_bUsePassword;
	UPROPERTY(BlueprintReadOnly)
		class UMainMenuWidget* m_ParentWidget;

public:
	void SetSessionInformation(const FString& Name, const int32& CurrentPlayer, const int32& MaxPlayer, const bool& UsePassword);


};
