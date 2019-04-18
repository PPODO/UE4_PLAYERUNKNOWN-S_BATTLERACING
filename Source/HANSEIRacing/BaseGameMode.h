#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SocketComponent.h"
#include <sstream>
#include "BaseGameMode.generated.h"

UENUM(BlueprintType)
enum class EPACKETFAILEDTYPE : uint8 {
	EPFT_FAILED,
	EPFT_SUCCEED,
	EPFT_EXIST,
	EPFT_COUNT
};

UCLASS()
class HANSEIRACING_API ABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABaseGameMode();

public:
	virtual void RecvDataProcessing(uint8* RecvBuffer, int32& RecvBytes);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;

protected:
	int32 m_Port;
	FString m_SocketName;

private:
	class FSocketComponent* m_Socket;
	bool m_bIsConnected;

public:
	FORCEINLINE FSocketComponent* GetSocket() const { return m_Socket; }

public:
	UFUNCTION(BlueprintCallable)
		bool GetIsConnected() { return m_bIsConnected; }

};