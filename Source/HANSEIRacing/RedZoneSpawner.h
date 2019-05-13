#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "RedZoneSpawner.generated.h"

USTRUCT(BlueprintType)
struct FCoordinateInformation : public FTableRowBase {
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Coordinate")
		float X;
	UPROPERTY(BlueprintReadWrite, Category = "Coordinate")
		float Y;
	UPROPERTY(BlueprintReadWrite, Category = "Coordinate")
		float Delay;

public:
	FCoordinateInformation() : X(0.f), Y(0.f), Delay(0.f) {};

};

UCLASS()
class HANSEIRACING_API ARedZoneSpawner : public AActor
{
	GENERATED_BODY()
	
public:
	ARedZoneSpawner();

protected:
	virtual void Tick(float DeltaTime) override;

private:
	TSubclassOf<class ABombProjectile> m_BombProjectileClass;
	TArray<FCoordinateInformation*> m_CoordInformation;

private:
	bool m_bActivateRedZone;

private:
	UPROPERTY(BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
		int32 m_SplinePoint;

public:
	FORCEINLINE void SetActivateRedZone(const bool& b, const int32& Point) { m_bActivateRedZone = b; m_SplinePoint = Point; }

private:
	UFUNCTION()
		void SpawnProjectileCallbackFunction();

public:
	UFUNCTION(BlueprintImplementableEvent)
		void SpawnBombProjectile(const FCoordinateInformation& CoordInformation);
		void SpawnBombProjectile_Implementation(const FCoordinateInformation& CoordInformation);

};
