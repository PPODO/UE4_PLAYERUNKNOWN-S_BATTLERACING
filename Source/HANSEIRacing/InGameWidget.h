#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameWidget.generated.h"

UCLASS()
class HANSEIRACING_API UInGameWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(BlueprintReadOnly)
		TMap<int32, class ADefaultVehicleCharacter*> m_CharacterClass;

private:
	bool m_bIsUpdateClassData = false;

public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdatePlayerList();
		void UpdatePlayerList_Implementation();

public:
	void SetCharacterClassData(const TMap<int32, class ADefaultVehicleCharacter*>& Data) { m_CharacterClass = Data; m_bIsUpdateClassData = true; }

};