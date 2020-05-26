#include "GameModeBase/InGameModeBase.h"
#include "GameStateBase/InGameStateBase.h"
#include "PlayerState/VehiclePlayerState.h"
#include "Vehicle/WheeledVehicle4W.h"
#include "PlayerController/VehiclePlayerController.h"
#include "GameInstance/DefaultGameInstance.h"
#include "UI/LobbyUI.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"

AInGameModeBase::AInGameModeBase() {
	::ConstructorHelpers::FClassFinder<AWheeledVehicle4W> BPVehicleClass(L"'/Game/Blueprint/BP_Vehicle'");

	if (BPVehicleClass.Succeeded()) {
		//DefaultPawnClass = BPVehicleClass.Class;
	}
	PlayerStateClass = AVehiclePlayerState::StaticClass();
	PlayerControllerClass = AVehiclePlayerController::StaticClass();
	GameStateClass = AInGameStateBase::StaticClass();
}

void AInGameModeBase::PostLogin(APlayerController* playerController) {
	Super::PostLogin(playerController);

	if (auto pc = Cast<AVehiclePlayerController>(playerController)) {
		mPlayerControllers.Add(pc->GetPlayerID(), pc);
	}
}

void AInGameModeBase::Logout(AController* exiting) {
	Super::Logout(exiting);

	if (auto pc = Cast<AVehiclePlayerController>(exiting)) {
		mPlayerControllers.Remove(pc->GetPlayerID());
	}
}

ETeamType AInGameModeBase::AllocateTeamType() {
	if (mCurrentNumberOfRedTeam < mMaxOfPeople) {
		++mCurrentNumberOfRedTeam;
		return ETeamType::Team_Red;
	}
	else if (mCurrentNumberOfBlueTeam < mMaxOfPeople) {
		++mCurrentNumberOfBlueTeam;
		return ETeamType::Team_Blue;
	}
	return ETeamType::Team_None;
}