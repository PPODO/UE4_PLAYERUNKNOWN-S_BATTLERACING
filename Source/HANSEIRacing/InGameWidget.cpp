#include "InGameWidget.h"
#include "DefaultVehicleCharacter.h"

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