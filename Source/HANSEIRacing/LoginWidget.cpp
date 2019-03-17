#include "LoginWidget.h"
#include "HANSEIRacingGameInstance.h"
#include "SocketComponent.h"

void ULoginWidget::SucceedLogin(std::stringstream& RecvStream) {
	int32 State = -1;
	RecvStream >> State;

	if (State == PACKET::ELF_SUCCEED) {
		if (m_GameInstance) {
			m_GameInstance->SetPlayerNickName(GetPlayerNickName(RecvStream));
			m_GameInstance->SetIsLogined(true);
		}
	}
	m_PacketState = (PACKET::ELOGINFAILED)State;
	m_bPopupWarningMessageBox = true;
}

void ULoginWidget::SucceedSignup(std::stringstream& RecvStream) {
	int32 State = -1;
	RecvStream >> State;

	m_PacketState = (PACKET::ESIGNUPFAILED)State;
	m_bPopupWarningMessageBox = true;
}

FString ULoginWidget::GetPlayerNickName(std::stringstream& RecvStream) {
	std::string NickName;
	RecvStream >> NickName;
	
	return FString(NickName.c_str());
}