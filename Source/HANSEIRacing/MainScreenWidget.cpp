#include "MainScreenWidget.h"
#include "SessionListWidget.h"
#include "HANSEIRacingGameInstance.h"

void UMainScreenWidget::NativeConstruct() {
	UUserWidget::NativeConstruct();

	m_bJoinSessionFailed = m_bCreateSessionFailed = m_bResetList = false;
	m_JoinSessionFailReason = PACKET::EJF_NONE;
	m_CreateSessionFailedReason = -1;
	m_MaxSessionCount = 0;
	m_CurrentPage = 1;
}

void UMainScreenWidget::SetSessionInformation(std::stringstream& RecvStream) {
	int32 IsFailed = -1;
	RecvStream >> IsFailed;
	RecvStream >> m_MaxSessionCount;
	
	if((EFAILED)IsFailed == EF_SUCCEED) {
		m_SessionInformations.m_Sessions.clear();
		RecvStream >> m_SessionInformations;
		m_bResetList = true;
	}
}

bool UMainScreenWidget::SucceedJoinSession(std::stringstream& RecvStream) {
	std::string SessionName;
	int32 Reason = PACKET::EJF_NONE;
	RecvStream >> Reason >> SessionName;

	if (Cast<UHANSEIRacingGameInstance>(GetGameInstance())) {
		Cast<UHANSEIRacingGameInstance>(GetGameInstance())->SetSessionName(ANSI_TO_TCHAR(SessionName.c_str()));
	}

	m_JoinSessionFailReason = Reason;
	if (m_JoinSessionFailReason != PACKET::EJF_SUCCEED) {
		m_bJoinSessionFailed = true;
		return false;
	}
	return true;
}

void UMainScreenWidget::FailedCreateSession(const bool& bFailed, const int32& FailedReason) {
	m_bCreateSessionFailed = bFailed;
	m_CreateSessionFailedReason = FailedReason;
}

TArray<UUserWidget*> UMainScreenWidget::CreateSessionLists() {
	TArray<UUserWidget*> Lists;
	if (IsInGameThread() && m_ListWidgetClass) {
		int32 m_SessionCount = m_SessionInformations.m_Sessions.size();
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