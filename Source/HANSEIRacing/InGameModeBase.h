#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SocketComponent.h"
#include "BaseGameMode.h"
#include <sstream>
#include "InGameModeBase.generated.h"

UCLASS()
class HANSEIRACING_API AInGameModeBase : public ABaseGameMode
{
	GENERATED_BODY()

public:
	AInGameModeBase();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;

public:
	virtual void RecvDataProcessing(TCHAR* RecvMessage);

public:
	void SendPlayerLocationAndRotation(const PLAYER::Vector& Location, const PLAYER::Vector& Rotation);

public:
	UFUNCTION(BlueprintCallable)
		void SendDisconnect();

public:
	UPROPERTY(BlueprintReadOnly)
		bool m_bSpawned;

private:
	void IsSucceedJoinSession(std::stringstream&);
	bool SendMessageToServerAboutConnectState(const FString& SessionName, const FString& NickName, uint32 UniqueKey);
	void DisconnectOtherPlayer(std::stringstream&);
	void UpdateCharacterInformation(std::stringstream&);

private:
	void SpawnCharacter();
	void UpdatePlayerLocationAndRotation();

private:
	class AHANSEIRacginPlayerController* m_PlayerController;
	class UHANSEIRacingGameInstance* m_GameInstance;
	TArray<AActor*> m_SpawnPoints;
	TMap<uint32, class ADefaultCharacter*> m_Players;

private:
	APawn* m_MyCharacter;
	bool m_bSpawnNewPlayer;

private:
	PLAYER::CharacterInformation m_PlayersInformation;
	
};