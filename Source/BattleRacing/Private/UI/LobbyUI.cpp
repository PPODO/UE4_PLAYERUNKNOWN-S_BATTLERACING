#include "UI/LobbyUI.h"
#include "GameStateBase/InGameStateBase.h"
#include "Engine/World.h"

void ULobbyUI::NativeConstruct() {
	Super::NativeConstruct();

	if (auto gameState = Cast<AInGameStateBase>(GetWorld()->GetGameState())) {
		gameState->mOnNewPlayerEntered.AddDynamic(this, &ULobbyUI::OnNewPlayerEntered);
	}
}

void ULobbyUI::OnNewPlayerEntered(const FLobbyUserInformation& newPlayerInformation) {
	UE_LOG(LogTemp, Error, L"QWEWEQQEW");
}