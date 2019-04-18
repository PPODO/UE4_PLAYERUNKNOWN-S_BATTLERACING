#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawner.generated.h"

UCLASS()
class HANSEIRACING_API AItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:
	AItemSpawner();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		class UStaticMeshComponent* m_BoardMesh;

	UPROPERTY(EditAnywhere)
		int32 m_MaxItemSpawnCount;

};