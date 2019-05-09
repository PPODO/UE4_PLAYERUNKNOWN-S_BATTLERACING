#include "InGameWidget.h"
#include "DefaultVehicleCharacter.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UInGameWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsInGameThread()) {
		if (m_bIsUpdateClassData) {
			UpdatePlayerList();
			m_bIsUpdateClassData = false;
		}
	}
}

void UInGameWidget::SetCharacterClassData(const TMap<int32, ADefaultVehicleCharacter*>& Data) {
	TArray<ADefaultVehicleCharacter*> PlayerList;

	for (auto Iterator : Data) {
		if (IsValid(Iterator.Value)) {
			PlayerList.Add(Iterator.Value);
		}
	}

	Algo::Sort(PlayerList, [&](const ADefaultVehicleCharacter* Left, const ADefaultVehicleCharacter* Right) { 
		if (IsValid(Left) && IsValid(Right)) {
			return Left->GetPlayerCurrentRank() < Right->GetPlayerCurrentRank();
		}
		return false;
	});

	m_CharacterClass = PlayerList;
	m_bIsUpdateClassData = true;
}

void UInGameWidget::SetCountdownTimer(const int32& Count, const FString& Message, const TFunction<void()>& Callback) {
	m_Count = Count; m_Message = Message; m_CountdownFinishedCallback = Callback; m_bIsDisableTimer = false;
	GetWorld()->GetTimerManager().SetTimer(m_CountdownTimerHandle, this, &UInGameWidget::TimerCallbackFunction, 1.f, true, 1.f);
}

void UInGameWidget::TimerCallbackFunction() {
	if (m_Count > 0) {
		m_Count--;
	}
	else {
		if (m_CountdownFinishedCallback) {
			m_CountdownFinishedCallback();
		}
		m_Count = 0;
		m_bIsDisableTimer = true;
		GetWorld()->GetTimerManager().ClearTimer(m_CountdownTimerHandle);
	}
}