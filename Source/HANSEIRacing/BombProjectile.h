#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BombProjectile.generated.h"

UCLASS()
class HANSEIRACING_API ABombProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	ABombProjectile();

private:
	UPROPERTY()
		class USphereComponent* m_SphereCollision;
	UPROPERTY()
		class URadialForceComponent* m_RadialForceComponent;
	UPROPERTY()
		class UChildActorComponent* m_RocketActorComponent;
	UPROPERTY()
		class UProjectileMovementComponent* m_ProjectileMovement;
	UPROPERTY()
		class UExplosionComponent* m_ExplosionComponent;

private:
	float m_ProjectileSpeed;

private:
	UFUNCTION()
		void OnComponentHit(class UPrimitiveComponent* HitComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
