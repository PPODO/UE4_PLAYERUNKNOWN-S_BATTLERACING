#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HANSEIRacginPlayerController.generated.h"

UCLASS()
class HANSEIRACING_API AHANSEIRacginPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AHANSEIRacginPlayerController();

public:
	UFUNCTION(BlueprintCallable)
		FORCEINLINE bool GetVisiblePauseUI() const { return m_bVisiblePauseUI; }
	UFUNCTION(BlueprintCallable)
		void SetVisiblePauseUI(bool b) { m_bVisiblePauseUI = b; }

private:
	bool m_bVisiblePauseUI;

};
