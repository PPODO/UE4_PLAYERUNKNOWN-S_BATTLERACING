#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HomingProjectile.generated.h"

UCLASS()
class BATTLERACING_API AHomingProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	AHomingProjectile();

protected:
	virtual void BeginPlay() override;

public:
	void SetHomingTarget(AActor* target);

private:
	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* mSceneComponent;
	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		class UCapsuleComponent* mCollisionComponent;
	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		class UChildActorComponent* mChildActorComponent;
	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		class UProjectileMovementComponent* mProjectileMovementComponent;


};
