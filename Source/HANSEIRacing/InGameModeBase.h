#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
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

private:
	void IsSucceedJoinSession(std::stringstream&);
	void SendDisconnect();

private:
	void SpawnCharacter();

private:
	class APlayerController* m_PlayerController;
	class UHANSEIRacingGameInstance* m_GameInstance;
	TArray<AActor*> m_SpawnPoints;

private:
	bool m_bSpawnNewPlayer;
	APawn* m_MyCharacter;
	
};