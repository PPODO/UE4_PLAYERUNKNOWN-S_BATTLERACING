#include "MainScreenWidget.h"
#include "SessionListWidget.h"

void UMainScreenWidget::NativeConstruct() {
	UUserWidget::NativeConstruct();

	m_bResetList = false;
}

void UMainScreenWidget::SetSessionInformation(std::stringstream& RecvStream) {
	int32 IsFailed = -1;
	RecvStream >> IsFailed;
	
	if((PACKET::EFAILED)IsFailed == PACKET::EF_SUCCESSED) {
		m_SessionInformations.m_Sessions.clear();
		RecvStream >> m_SessionInformations;
		m_bResetList = true;
	}
}

TArray<UUserWidget*> UMainScreenWidget::CreateSessionLists() {
	TArray<UUserWidget*> Lists;
	if (IsInGameThread() && m_ListWidgetClass) {
		m_SessionCount = m_SessionInformations.m_Sessions.size();
		for (int32 i = 0; i < m_SessionCount; i++) {
			auto Widget = CreateWidget<UUserWidget>(GetWorld(), m_ListWidgetClass);
			if (Widget && Cast<USessionListWidget>(Widget)) {
				SESSION::Session Info = m_SessionInformations.m_Sessions[i];

				Cast<USessionListWidget>(Widget)->SetSessionInfomation(ANSI_TO_TCHAR(Info.m_SessionName.c_str()), Info.m_CurrentPlayer, Info.m_MaxPlayer, Info.m_bUsePassword);
				Cast<USessionListWidget>(Widget)->SetParentWidgetClass(this);
				Lists.Add(Widget);
			}
		}
	}
	return Lists;
}