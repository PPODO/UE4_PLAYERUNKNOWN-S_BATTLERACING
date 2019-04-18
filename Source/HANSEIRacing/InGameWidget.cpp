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