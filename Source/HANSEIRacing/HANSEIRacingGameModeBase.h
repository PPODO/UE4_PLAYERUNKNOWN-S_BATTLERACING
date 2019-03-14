#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HANSEIRacingGameInstance.h"
#include "BaseGameMode.h"
#include <sstream>
#include "HANSEIRacingGameModeBase.generated.h"

UCLASS()
class HANSEIRACING_API AHANSEIRacingGameModeBase : public ABaseGameMode {
	GENERATED_BODY()

public:
	AHANSEIRacingGameModeBase();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	virtual void RecvDataProcessing(TCHAR* RecvMessage) override;

private:
	bool CreateSessionSucceed(std::stringstream& RecvStream);
	void DisconnectSucceed(std::stringstream& RecvStream);

private:
	class UHANSEIRacingGameInstance* m_GameInstance;
	class ULoginWidget* m_LoginWidget;
	class UMainScreenWidget* m_MainWidget;

private:
	bool m_bDisconnected;

public:
	UFUNCTION(BlueprintCallable)
		void SendPlayerLoginInformationToServer(const EPACKETMESSAGE& PackType, const FString& ID, const FString& Password);
	UFUNCTION(BlueprintCallable)
		void SendPlayerSignUpInformationToServer(const EPACKETMESSAGE& PackType, const FString& NickName, const FString& ID, const FString& Password);
	UFUNCTION(BlueprintCallable)
		void SendCreateSessionInformationToServer(const EPACKETMESSAGE& PackType, const FString& SessionName, const int32 MaxPlayer, const bool bUsePassword, const FString& Password = FString(""));
	UFUNCTION(BlueprintCallable)
		void SendAllSessionInformtaionToServer(const EPACKETMESSAGE& PacketType, const int32 MinLimit);
	UFUNCTION(BlueprintCallable)
		void SendJoinSessionToServer(const EPACKETMESSAGE& PacketType, const FString& SessionName, const bool bUsePassword = false, const FString& Password = "");
	UFUNCTION(BlueprintCallable)
		void SendDisconnectToServer(const EPACKETMESSAGE& PacketType);

public:
	UFUNCTION(BlueprintCallable)
		void SetLoginWidgetClass(class UUserWidget* Widget);
	UFUNCTION(BlueprintCallable)
		void SetMainScreenWidgetClass(class UUserWidget* Widget);

};