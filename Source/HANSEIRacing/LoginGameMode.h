#pragma once

#include "CoreMinimal.h"
#include "BaseGameMode.h"
#include "LoginGameMode.generated.h"

UENUM(BlueprintType)
enum class EPACKETMESSAGEFORLOGINTYPE : uint8 {
	EPMLT_LOGIN,
	EPMLT_NEWACCOUNT,
	EPMLT_CREATESESSION,
	EPMLT_JOINSESSION,
	EPMLT_GETSESSION,
	EPMLT_COUNT
};

UCLASS()
class HANSEIRACING_API ALoginGameMode : public ABaseGameMode
{
	GENERATED_BODY()
	
public:
	ALoginGameMode();

public:
	virtual void RecvDataProcessing(uint8* RecvBuffer, int32& RecvBytes) override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;

private:
	class ULoginWidget* m_LoginWidget;
	class UMainMenuWidget* m_MenuWidget;
	class UHANSEIRacingGameInstance* m_GameInstance;
	bool m_bIsDisConnect;

private:
	// FROM Server
	void IsSucceedLogin(std::stringstream& RecvStream);
	void IsSucceedCreateAccount(std::stringstream& RecvStream);
	void IsSucceedCreateSession(std::stringstream& RecvStream);
	void IsSucceedGetSessionList(std::stringstream& RecvStream);
	void IsSucceedJoinSession(std::stringstream& RecvStream);

public:
	// TO Server
	UFUNCTION(BlueprintCallable)
		void SendLoginToServer(const FString& ID, const FString& Password);

	UFUNCTION(BlueprintCallable)
		void SendCreateAccountToServer(const FString& UserName, const FString& ID, const FString& Password);

	UFUNCTION(BlueprintCallable)
		void SendCreateSessionToServer(const FString& SessionName, const int32& MaxPlayer, const bool& UsePassword, const FString& Password);

	UFUNCTION(BlueprintCallable)
		void SendGetSessionInformationToServer(const int32& CurrentPage);

	UFUNCTION(BlueprintCallable)
		void SendJoinSessionToServer(const FString& SessionName, const bool& UsePassword, const FString& Password);

public:
	UFUNCTION(BlueprintCallable)
		void SetLoginWidgetClass(class ULoginWidget* Class) { m_LoginWidget = Class; }

	UFUNCTION(BlueprintCallable)
		void SetMainWidgetClass(class UMainMenuWidget* Class) { m_MenuWidget = Class; };

};