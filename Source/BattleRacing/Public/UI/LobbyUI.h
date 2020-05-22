#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Includes/DefineEnums.h"
#include "LobbyUI.generated.h"

UCLASS()
class BATTLERACING_API ULobbyUI : public UUserWidget {
	GENERATED_BODY()
public:
	UFUNCTION(NetMulticast, Reliable)
		void AddUser(ETeamType teamType, FName UserNameW);

};