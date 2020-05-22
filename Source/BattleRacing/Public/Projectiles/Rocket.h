#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Rocket.generated.h"

UCLASS()
class BATTLERACING_API ARocket : public AActor
{
	GENERATED_BODY()
	
public:
	ARocket();

private:
	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* mRocketMeshComponent;
	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* mFlareParticle;
	UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* mSmokeParticle;

};