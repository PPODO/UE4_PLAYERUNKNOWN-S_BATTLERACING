#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBox.generated.h"

UCLASS()
class BATTLERACING_API AItemBox : public AActor {
	GENERATED_BODY()

public:
	AItemBox();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
		void OnComponentOverlappedBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(NetMulticast, Reliable)
		void UpdateItemBoxState(bool bIsActive);

	void SetupRespawnTimer();

private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* mItemBoxMesh;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* mItemBoxCollisionComponent;

private:
	struct FTimerHandle mTimerHandle;

private:
	FRotator mRotateDirection;

};