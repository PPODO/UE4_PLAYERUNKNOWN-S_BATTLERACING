#include "LobbyWidget.h"
#include "InGameMode.h"

void ULobbyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsInGameThread()) {
		if (m_bIsUpdateList) {
			UpdateList();
			m_bIsUpdateList = false;
		}
	}
}

void ULobbyWidget::SetPlayerList(const std::vector<struct GAMEPACKET>& List) {
	m_bIsCanStart = false;
	m_PlayerList.Empty();

	int32 CountReady = 0;
	for (auto It : List) {
		if (It.m_bIsReady) {
			CountReady++;
		}
		m_PlayerList.Add(FPlayerListInformation(ANSI_TO_TCHAR(It.m_PlayerNickName), It.m_bIsReady));
		m_bIsUpdateList = true;
	}
	if (CountReady == m_PlayerList.Num() - 1) {
		m_bIsCanStart = true;
	}
}