#include "PlayerController/VehiclePlayerController.h"
#include "GameStateBase/InGameStateBase.h"
#include "GameInstance/DefaultGameInstance.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "UI/LobbyUI.h"
#include "Camera/CameraActor.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

AVehiclePlayerController::AVehiclePlayerController() {
	SetReplicates(true);
}

void AVehiclePlayerController::BeginPlay() {
	Super::BeginPlay();

}

void AVehiclePlayerController::Tick(float deltaTime) {
	Super::Tick(deltaTime);
	
}

int32 AVehiclePlayerController::GetPlayerID() {
	if (auto ps = GetPlayerState<APlayerState>())
		return ps->PlayerId;
	return -1;
};
