#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "StartPoint.generated.h"

UCLASS()
class HANSEIRACING_API AStartPoint : public ATargetPoint
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
		int32 m_StartPointTag;

};