#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rocket.generated.h"

UCLASS()
class HANSEIRACING_API ARocket : public AActor
{
	GENERATED_BODY()
	
public:	
	ARocket();

private:
	UPROPERTY()
		class USceneComponent* m_SceneCollision;
	UPROPERTY()
		class UStaticMeshComponent* m_RocketMeshComp;
	UPROPERTY()
		class UParticleSystemComponent* m_TrailParticle;
	UPROPERTY()
		class UParticleSystemComponent* m_FlareParticle;

public:


};
