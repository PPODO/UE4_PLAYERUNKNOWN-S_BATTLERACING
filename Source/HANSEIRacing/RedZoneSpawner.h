#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RedZoneSpawner.generated.h"

UCLASS()
class HANSEIRACING_API ARedZoneSpawner : public AActor
{
	GENERATED_BODY()
	
public:
	ARedZoneSpawner();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;

private:
	TSubclassOf<class ABombProjectile> m_BombProjectileClass;
	FTimerHandle m_SpawnProjectileTimerHandle;

private:
	UFUNCTION()
		void SpawnProjectileCallbackFunction();

};
