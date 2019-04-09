#include "SessionList.h"
#include "MainMenuWidget.h"

void USessionList::SetSessionInformation(const FString& Name, const int32 & CurrentPlayer, const int32 & MaxPlayer, const bool & UsePassword) {
	m_SessionName = Name;
	m_CurrentPlayer = CurrentPlayer;
	m_MaxPlayer = MaxPlayer;
	m_bUsePassword = UsePassword;
}