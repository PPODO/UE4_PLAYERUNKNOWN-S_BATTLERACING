#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DefaultGameInstance.generated.h"

UCLASS()
class BATTLERACING_API UDefaultGameInstance : public UGameInstance {
	GENERATED_BODY()
public:
	UDefaultGameInstance();

public:
	UFUNCTION(BlueprintCallable)
		void CreateLobbyWidget();
	UFUNCTION(BlueprintCallable)
		void RemoveLobbyWidget();

public:
	FORCEINLINE class ULobbyUI* GetLobbyUIInstance() { return mLobbyUIInstance; }

private:
	TSubclassOf<class ULobbyUI> mLobbyUISubclass;
	class ULobbyUI* mLobbyUIInstance{ nullptr };

};