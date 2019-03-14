#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <sstream>
#include "LoginWidget.generated.h"

UCLASS()
class HANSEIRACING_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	FString GetPlayerNickName(std::stringstream& RecvStream);

public:
	void SucceedLogin(std::stringstream& RecvStream);
	void SucceedSignup(std::stringstream& RecvStream);

public:
	UPROPERTY(BlueprintReadWrite)
		class UHANSEIRacingGameInstance* m_GameInstance;
	UPROPERTY(BlueprintReadWrite)
		int32 m_PacketState = -1;
	UPROPERTY(BlueprintReadWrite)
		bool m_bPopupWarningMessageBox = false;

private:
	

	
};