#include "GameModeBase/InGameModeBase.h"
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
	PlayerControllerClass = AVehiclePlayerController::StaticClass();
}

void AInGameModeBase::PostLogin(APlayerController* playerController) {
	Super::PostLogin(playerController);
	
	if (auto pc = Cast<AVehiclePlayerController>(playerController)) {
		ETeamType teamType = mPlayerControllers.Num(ETeamType::Team_Red) != mMaxNumberOfPeole ? ETeamType::Team_Red : ETeamType::Team_Blue;
		mPlayerControllers.Add(teamType, pc);
		pc->SetTeamType(teamType);
		if (auto gameInstance = Cast<UDefaultGameInstance>(GetWorld()->GetGameInstance())) {
			gameInstance->GetLobbyUIInstance();

		}
	}
}

void AInGameModeBase::Logout(AController* exiting) {
	Super::Logout(exiting);

	if (auto pc = Cast<AVehiclePlayerController>(exiting)) {
		mPlayerControllers.Remove(pc->GetTeamType(), pc);
	}
}