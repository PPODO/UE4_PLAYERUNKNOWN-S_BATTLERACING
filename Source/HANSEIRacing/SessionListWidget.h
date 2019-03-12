#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionListWidget.generated.h"

UCLASS()
class HANSEIRACING_API USessionListWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
		FString m_SessionName;
	UPROPERTY(BlueprintReadWrite)
		int32 m_CurrentPlayer;
	UPROPERTY(BlueprintReadWrite)
		int32 m_MaxPlayer;
	UPROPERTY(BlueprintReadWrite)
		bool m_UsePassword;
	
	UPROPERTY(BlueprintReadOnly)
		UUserWidget* m_ParentWidget;

public:
	void SetSessionInfomation(const FString& Name, const int32& Current, const int32& Max, const bool Use);
	void SetParentWidgetClass(UUserWidget* Widget);


	
};