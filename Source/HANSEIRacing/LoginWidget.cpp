#include "LoginWidget.h"
#include "HANSEIRacingGameInstance.h"
#include "Networking.h"
#include "engine/Engine.h"

void ULoginWidget::SuccesedLogin(std::stringstream& RecvStream) {
	int32 State = -1;
	RecvStream >> State;

	if (State == PACKET::EF_SUCCESSED) {
		if (m_GameInstance) {
			m_GameInstance->SetPlayerNickName(GetPlayerNickName(RecvStream));
		}
	}
	m_PacketState = (EPACKETSTATE)State;
	m_bPopupWarningMessageBox = true;
}

FString ULoginWidget::GetPlayerNickName(std::stringstream& RecvStream) {
	std::string NickName;
	RecvStream >> NickName;
	
	return FString(NickName.c_str());
}