#include "GameStateBase/InGameStateBase.h"
#include "GameModeBase/InGameModeBase.h"
#include "Net/UnrealNetwork.h"

AInGameStateBase::AInGameStateBase() {
	SetReplicates(true);
}

void AInGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AInGameStateBase, mUsersInformation);
}

void AInGameStateBase::OnNewPlayerEntered(int32 playerID, const FString& nickname) {
	if (!HasAuthority()) {
		UE_LOG(LogNet, Error, L"The client is not authorized to call the function.");
		return;
	}
	if (auto gameMode = Cast<AInGameModeBase>(AuthorityGameMode)) {
		auto teamType = gameMode->AllocateTeamType();
		if (teamType != ETeamType::Team_None) {
			mUsersInformation.Add(FLobbyUserInformation(playerID, nickname, teamType));
			return;
		}
		// Error Handler
	}
}

void AInGameStateBase::OnRep_UserInformation() {
	if (!HasAuthority() && mOnNewPlayerEntered.IsBound()) {
		mOnNewPlayerEntered.Broadcast(mUsersInformation.Last());
	}
}