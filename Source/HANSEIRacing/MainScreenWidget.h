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
	UPROPERTY(BlueprintReadWrite)
		bool m_bResetList;
	UPROPERTY(BlueprintReadWrite)
		TSubclassOf<UUserWidget> m_ListWidgetClass;

public:
	void SetSessionInformation(std::stringstream& RecvStream);

private:
	int32 m_SessionCount;
	SESSION::SessionInformation m_SessionInformations;

};