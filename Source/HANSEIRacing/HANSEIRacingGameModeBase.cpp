#include "HANSEIRacingGameModeBase.h"
#include "HANSEIRacingGameInstance.h"
#include "SocketComponent.h"
#include "MainScreenWidget.h"
#include "LoginWidget.h"
#include <sstream>

AHANSEIRacingGameModeBase::AHANSEIRacingGameModeBase() : m_GameInstance(nullptr), m_LoginWidget(nullptr), m_MainWidget(nullptr) {

	PrimaryActorTick.bCanEverTick = true;
}

void AHANSEIRacingGameModeBase::BeginPlay() {
	ABaseGameMode::BeginPlay();

	m_GameInstance = Cast<UHANSEIRacingGameInstance>(GetGameInstance());
}

void AHANSEIRacingGameModeBase::Tick(float DeltaTime) {
	ABaseGameMode::Tick(DeltaTime);

}

void AHANSEIRacingGameModeBase::BeginDestroy() {
	ABaseGameMode::BeginDestroy();

}

void AHANSEIRacingGameModeBase::RecvDataProcessing(TCHAR* RecvMessage) {
	std::stringstream RecvStream(TCHAR_TO_ANSI(RecvMessage));
	int32 PacketType = -1, IsSuccess = PACKET::EF_FAILED;
	RecvStream >> PacketType;

	switch (PacketType) {
	case PM_SIGNUP:

		break;
	case PM_LOGIN:
		if (m_LoginWidget) {
			m_LoginWidget->SuccesedLogin(RecvStream);
		}
		break;
	case PM_SESSIONLIST:
		if (m_MainWidget) {
			m_MainWidget->SetSessionInformation(RecvStream);
		}
		break;

	default:
		break;
	}
}

void AHANSEIRacingGameModeBase::SendPlayerLoginInformationToServer(const EPACKETMESSAGE& PackType, const FString& ID, const FString& Password) {
	std::stringstream SendStream;

	SendStream << PackType << std::endl;
	SendStream << TCHAR_TO_ANSI(*ID) << std::endl;
	SendStream << TCHAR_TO_ANSI(*Password) << std::endl;

	GetClientSocket()->Send(SendStream.str().c_str(), SendStream.str().length());
}

void AHANSEIRacingGameModeBase::SendPlayerSignUpInformationToServer(const EPACKETMESSAGE& PackType, const FString& NickName, const FString& ID, const FString& Password) {
	std::stringstream SendStream;

	SendStream << PackType << std::endl;
	SendStream << TCHAR_TO_ANSI(*NickName) << std::endl;
	SendStream << TCHAR_TO_ANSI(*ID) << std::endl;
	SendStream << TCHAR_TO_ANSI(*Password) << std::endl;

	GetClientSocket()->Send(SendStream.str().c_str(), SendStream.str().length());
}

void AHANSEIRacingGameModeBase::SendCreateSessionInformationToServer(const EPACKETMESSAGE & PackType, const FString & SessionName, const int32 MaxPlayer, const bool bUsePassword, const FString & Password) {
	std::stringstream SendStream;

	SendStream << PackType << std::endl;
	SendStream << TCHAR_TO_ANSI(*SessionName) << std::endl;
	SendStream << MaxPlayer << std::endl;
	SendStream << bUsePassword << std::endl;
	SendStream << TCHAR_TO_ANSI(*Password) << std::endl;

	GetClientSocket()->Send(SendStream.str().c_str(), SendStream.str().length());
}

void AHANSEIRacingGameModeBase::SendAllSessionInformtaionToServer(const EPACKETMESSAGE& PacketType) {
	std::stringstream SendStream;
	
	SendStream << PacketType << std::endl;

	GetClientSocket()->Send(SendStream.str().c_str(), SendStream.str().length());
}

void AHANSEIRacingGameModeBase::SendJoinSessionToServer(const EPACKETMESSAGE& PacketType, const FString & SessionName, const bool bUsePassword, const FString & Password) {
	std::stringstream SendStream;

	SendStream << PacketType << std::endl << TCHAR_TO_ANSI(*SessionName) << std::endl << bUsePassword << std::endl << TCHAR_TO_ANSI(*Password) << std::endl;
	
	GetClientSocket()->Send(SendStream.str().c_str(), SendStream.str().length());
}

void AHANSEIRacingGameModeBase::SetLoginWidgetClass(UUserWidget* Widget) {
	m_LoginWidget = Cast<ULoginWidget>(Widget);
}

void AHANSEIRacingGameModeBase::SetMainScreenWidgetClass(UUserWidget * Widget) {
	m_MainWidget = Cast<UMainScreenWidget>(Widget);
}