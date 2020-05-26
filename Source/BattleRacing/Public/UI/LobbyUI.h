#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Includes/DefineStructure.h"
#include "LobbyUI.generated.h"

UCLASS()
class BATTLERACING_API ULobbyUI : public UUserWidget {
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override final;

public:
	UFUNCTION()
		void OnNewPlayerEntered(const FLobbyUserInformation& newPlayerInformation);

};