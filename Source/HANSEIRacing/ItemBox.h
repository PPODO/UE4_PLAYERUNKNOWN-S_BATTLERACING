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
	UPROPERTY()
		class UStaticMeshComponent* m_ItemBoxMesh;

private:
	FRotator m_RotateDirection;

};
