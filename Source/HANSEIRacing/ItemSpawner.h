#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawner.generated.h"

enum class EITEMTYPE : uint8;

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
		int32 m_MaxItemSpawnCount;
	UPROPERTY(EditAnywhere, Category = "Unique Key")
		int32 m_UniqueID;

private:
	UPROPERTY()
		class UStaticMeshComponent* m_BoardMesh;
	UPROPERTY()
		TArray<class UChildActorComponent*> m_ItemBoxActors;

public:
	void FindItemIndexAndReset(const int32& ItemIndex);

public:
	FORCEINLINE int32 GetSpawnerUniqueID() const { return m_UniqueID; }

};