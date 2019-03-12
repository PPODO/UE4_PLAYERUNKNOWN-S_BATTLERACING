#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "InGameModeBase.generated.h"

UCLASS()
class HANSEIRACING_API AInGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AInGameModeBase();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;

private:
	class FSocketComponent* m_ClientSocket;
	
};
