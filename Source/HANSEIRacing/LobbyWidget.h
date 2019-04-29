#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <vector>
#include "LobbyWidget.generated.h"

USTRUCT(BlueprintType)
struct FPlayerListInformation {
	GENERATED_USTRUCT_BODY()

public:
	FPlayerListInformation() : m_PlayerName(), m_bIsReady(false) {};
	FPlayerListInformation(const FString&& PlayerName, const bool& IsReady) : m_PlayerName(PlayerName), m_bIsReady(IsReady) {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
		FString m_PlayerName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Information")
		bool m_bIsReady;

};

UCLASS()
class HANSEIRACING_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	bool m_bIsUpdateList = false;

public:
	UPROPERTY(BlueprintReadOnly)
		TArray<FPlayerListInformation> m_PlayerList;
	UPROPERTY(BlueprintReadOnly)
		bool m_bIsCanStart = false;

public:
	void SetPlayerList(const std::vector<struct GAMEPACKET>& List);

public:
	UFUNCTION(BlueprintImplementableEvent)
		void UpdateList();
		void UpdateList_Implementation();

};