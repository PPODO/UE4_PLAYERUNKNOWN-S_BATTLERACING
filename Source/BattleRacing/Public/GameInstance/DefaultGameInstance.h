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
	UFUNCTION(BlueprintCallable)
		void SetLobbyWidgetVisibility(enum ESlateVisibility visibility);

public:
	FORCEINLINE class ULobbyUI* GetLobbyUIInstance() { return mLobbyUIInstance; }
	FORCEINLINE const FString& GetPlayerNickname() { return mPlayerNickname; }

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		FString mPlayerNickname;

private:
	TSubclassOf<class ULobbyUI> mLobbyUISubclass;
	class ULobbyUI* mLobbyUIInstance{ nullptr };

};