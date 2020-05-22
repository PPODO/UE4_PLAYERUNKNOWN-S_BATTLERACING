#include "GameInstance/DefaultGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameModeBase/InGameModeBase.h"
#include "UI/LobbyUI.h"
#include "Engine/World.h"

UDefaultGameInstance::UDefaultGameInstance() {
	::ConstructorHelpers::FClassFinder<ULobbyUI> LobbyUIClass(L"'/Game/UI/BP_LobbyUI'");

	if (LobbyUIClass.Succeeded()) {
		mLobbyUISubclass = LobbyUIClass.Class;
	}
}

void UDefaultGameInstance::CreateLobbyWidget() {
	if (auto LobbyWidget = CreateWidget<ULobbyUI>(this, mLobbyUISubclass)) {
		mLobbyUIInstance = LobbyWidget;
		mLobbyUIInstance->AddToViewport();
		if (auto TargetViewCameraActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass())) {
			GetFirstLocalPlayerController()->SetViewTarget(TargetViewCameraActor);
		}
	}
}

void UDefaultGameInstance::RemoveLobbyWidget() {
	if (mLobbyUIInstance) {
		mLobbyUIInstance->RemoveFromParent();
	}
}