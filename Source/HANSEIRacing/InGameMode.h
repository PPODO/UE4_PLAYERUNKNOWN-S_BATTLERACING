#pragma once

#include "CoreMinimal.h"
#include "BaseGameMode.h"
#include "InGameMode.generated.h"

UENUM(BlueprintType)
enum class EPACKETMESSAGEFORGAMETYPE : uint8 {
	EPMGT_JOIN,
	EPMGT_NEWPLAYER,
	EPMGT_UPDATE,
	EPMGT_DISCONNECT,
	EPMGT_DISCONNECTOTHER,
	EPMGT_COUNT
};

struct GAMEPACKET {
	EPACKETMESSAGEFORGAMETYPE m_MessageType;
	EPACKETFAILEDTYPE m_FailedReason;

public:
	GAMEPACKET() : m_MessageType(EPACKETMESSAGEFORGAMETYPE::EPMGT_COUNT), m_FailedReason(EPACKETFAILEDTYPE::EPFT_COUNT) {};

};

UCLASS()
class HANSEIRACING_API AInGameMode : public ABaseGameMode
{
	GENERATED_BODY()

public:
	AInGameMode();

public:
	virtual void RecvDataProcessing(uint8* RecvBuffer) override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;

private:
	class UHANSEIRacingGameInstance* m_GameInstance;

public:
	// FROM Server
	void IsSucceedJoinGame(GAMEPACKET*& Packet);

public:
	// TO Server
	void SendJoinGameToServer();
	
	GAMEPACKET* Packet;

};