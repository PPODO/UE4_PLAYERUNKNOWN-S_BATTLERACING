#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGameMode.h"
#include "HANSEIRacingGameModeBase.generated.h"

UENUM(BlueprintType)
enum EPACKETMESSAGE {
	PM_SIGNUP,
	PM_LOGIN,
	PM_CREATESESSION,
	PM_SESSIONLIST,
	PM_JOINSESSION
};

UCLASS()
class HANSEIRACING_API AHANSEIRacingGameModeBase : public ABaseGameMode {
	GENERATED_BODY()

public:
	AHANSEIRacingGameModeBase();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;

public:
	virtual void RecvDataProcessing(TCHAR* RecvMessage) override;

private:
	class UHANSEIRacingGameInstance* m_GameInstance;
	class ULoginWidget* m_LoginWidget;
	class UMainScreenWidget* m_MainWidget;

public:
	UFUNCTION(BlueprintCallable)
		void SendPlayerLoginInformationToServer(const EPACKETMESSAGE& PackType, const FString& ID, const FString& Password);
	UFUNCTION(BlueprintCallable)
		void SendPlayerSignUpInformationToServer(const EPACKETMESSAGE& PackType, const FString& NickName, const FString& ID, const FString& Password);
	UFUNCTION(BlueprintCallable)
		void SendCreateSessionInformationToServer(const EPACKETMESSAGE& PackType, const FString& SessionName, const int32 MaxPlayer, const bool bUsePassword, const FString& Password = FString(""));
	UFUNCTION(BlueprintCallable)
		void SendAllSessionInformtaionToServer(const EPACKETMESSAGE& PacketType);
	UFUNCTION(BlueprintCallable)
		void SendJoinSessionToServer(const EPACKETMESSAGE& PacketType, const FString& SessionName, const bool bUsePassword = false, const FString& Password = "");

public:
	UFUNCTION(BlueprintCallable)
		void SetLoginWidgetClass(class UUserWidget* Widget);
	UFUNCTION(BlueprintCallable)
		void SetMainScreenWidgetClass(class UUserWidget* Widget);

};