#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MultipleHomingProjectile.generated.h"

UCLASS()
class BATTLERACING_API AMultipleHomingProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	AMultipleHomingProjectile();

protected:
	virtual void BeginPlay() override;

private:
	void SpawnChildProjectiles();
	FTransform MakeProjectileTransform(int32 index, const FTransform& transform);

private:
	UFUNCTION(NetMulticast, Reliable)
		void SpawnProjectilesOnClients(const FTransform& transform, AActor* target);

private:
	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* mSceneComponent;
	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* mCollisionComponent;
	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		class UChildActorComponent* mChildActorComponent;

	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* mProjectileMovementComponent;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		int32 mMaxChildRocket = 12;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float mDistanceByProjectile = 25.f;

	struct FTimerHandle mSpawnChildProjectileHandler;

};
