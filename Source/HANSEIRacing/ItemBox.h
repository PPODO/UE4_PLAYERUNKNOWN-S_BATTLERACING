#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBox.generated.h"

UCLASS()
class HANSEIRACING_API AItemBox : public AActor
{
	GENERATED_BODY()
	
public:
	AItemBox();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
		class UBoxComponent* m_BoxCollisionComponent;
	UPROPERTY()
		class UStaticMeshComponent* m_ItemBoxMesh;

private:
	UFUNCTION()
		void OnComponentOverlapBegin(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	class AInGameMode* m_GameMode;

private:
	FRotator m_RotateDirection;
	int32 m_IndexNumber;
	bool m_bIsReset;

private:
	void ResetItemBox();

public:
	FORCEINLINE void SetIndexNumber(const int32& NewIndex) { m_IndexNumber = NewIndex; }
	FORCEINLINE void SetResetItemBox(const bool& bIsReset) { m_bIsReset = bIsReset; }

};