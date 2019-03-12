#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include <sstream>
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

public:
	virtual void RecvDataProcessing(TCHAR* RecvMessage);

private:
	class FSocketComponent* m_ClientSocket;

public:
	FORCEINLINE FSocketComponent* GetClientSocket() const { return m_ClientSocket; }

};