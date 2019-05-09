#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ExplosionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HANSEIRACING_API UExplosionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UExplosionComponent();

private:
	UPROPERTY()
		class UParticleSystem* m_ExplosionParticle;
	UPROPERTY()
		class USoundCue* m_ExplosionSound;

public:
	void Excute(const FVector& Location);
		
};
