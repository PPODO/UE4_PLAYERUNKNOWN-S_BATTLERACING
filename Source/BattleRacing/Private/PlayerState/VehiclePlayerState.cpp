#include "PlayerState/VehiclePlayerState.h"
#include "PlayerController/VehiclePlayerController.h"
#include "GameModeBase/InGameModeBase.h"
#include "GameStateBase/InGameStateBase.h"
#include "GameInstance/DefaultGameInstance.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

void AVehiclePlayerState::OnRep_PlayerId() {
	if (!HasAuthority() && HasLocalNetOwner()) {
		if (auto gameInstance = Cast<UDefaultGameInstance>(GetWorld()->GetGameInstance())) {
			OnInitializationSucceed(PlayerId, gameInstance->GetPlayerNickname());
		}
	}
}

void AVehiclePlayerState::OnInitializationSucceed_Implementation(int32 playerUniqueID, const FString& nickname) {
	SetPlayerName(nickname);
	if (auto gameState = Cast<AInGameStateBase>(GetWorld()->GetGameState())) {
		gameState->OnNewPlayerEntered(playerUniqueID, nickname);
	}
}

bool AVehiclePlayerState::OnInitializationSucceed_Validate(int32 playerUniqueID, const FString& nickname) {
	if (auto gameMode = Cast<AInGameModeBase>(GetWorld()->GetAuthGameMode())) {
		return gameMode->ValidatePlayerID(playerUniqueID);
	}
	return false;
}