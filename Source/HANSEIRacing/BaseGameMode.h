#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGameMode.generated.h"

UCLASS()
class HANSEIRACING_API ABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ABaseGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;

protected:
	int32 m_Port;
	FString m_SocketName;

public:
	virtual void RecvDataProcessing(TCHAR* RecvMessage);

public:
	UFUNCTION(BlueprintCallable)
		FORCEINLINE bool GetConnected() { return m_bIsConnectedToServer; }

public:
	FORCEINLINE void SetConnected(bool b) { m_bIsConnectedToServer = b; }
	FORCEINLINE class FSocketComponent* GetClientSocket() const { return m_Socket; }

public:
	UPROPERTY(BlueprintReadOnly)
		bool m_bFailedJoinGame;

private:
	class UHANSEIRacingGameInstance* m_GameInstance;
	class FSocketComponent* m_Socket;

private:
	bool m_bIsConnectedToServer;

};