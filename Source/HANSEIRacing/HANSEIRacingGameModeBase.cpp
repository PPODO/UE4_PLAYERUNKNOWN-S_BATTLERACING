#include "HANSEIRacingGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "MainScreenWidget.h"
#include "SocketComponent.h"
#include "LoginWidget.h"
#include <sstream>

AHANSEIRacingGameModeBase::AHANSEIRacingGameModeBase() : m_GameInstance(nullptr), m_LoginWidget(nullptr), m_MainWidget(nullptr), m_bDisconnected(false) {
	m_Port = 3510;
	m_SocketName = "LoginSocket";
	PrimaryActorTick.bCanEverTick = true;
}

void AHANSEIRacingGameModeBase::BeginPlay() {
	ABaseGameMode::BeginPlay();

	m_GameInstance = Cast<UHANSEIRacingGameInstance>(GetGameInstance());
}

void AHANSEIRacingGameModeBase::Tick(float DeltaTime) {
	ABaseGameMode::Tick(DeltaTime);

	if (IsInGameThread()) {
		if (m_bDisconnected && m_GameInstance) {
			UGameplayStatics::OpenLevel(this, "/Game/Maps/sex");
		}
	}
}

void AHANSEIRacingGameModeBase::RecvDataProcessing(TCHAR* RecvMessage) {
	std::stringstream RecvStream(TCHAR_TO_ANSI(RecvMessage));
	int32 PacketType = -1;
	RecvStream >> PacketType;

	switch (PacketType) {
	case PM_SIGNUP:
		if (m_LoginWidget) {
			m_LoginWidget->SucceedSignup(RecvStream);
		}
		break;
	case PM_LOGIN:
		if (m_LoginWidget) {
			m_LoginWidget->SucceedLogin(RecvStream);
		}
		break;
	case PM_CREATESESSION:
		if (CreateSessionSucceed(RecvStream)) {
			SendDisconnectToServer(PM_DISCONNECT);
		}
		break;
	case PM_SESSIONLIST:
		if (m_MainWidget) {
			m_MainWidget->SetSessionInformation(RecvStream);
		}
		break;
	case PM_JOINSESSION:
		if (m_MainWidget && m_MainWidget->SucceedJoinSession(RecvStream)) {
			SendDisconnectToServer(PM_DISCONNECT);
		}
		break;
	case PM_DISCONNECT:
		DisconnectSucceed(RecvStream);
		break;
	default:
		break;
	}
}

bool AHANSEIRacingGameModeBase::CreateSessionSucceed(std::stringstream& RecvStream) {
	int32 FailedReason = -1;
	std::string SessionName;
	RecvStream >> FailedReason >> SessionName;

	switch (FailedReason) {
	case PACKET::ENSF_EXIST:
	case PACKET::ENSF_FAILED:
		if (m_MainWidget) {
			m_MainWidget->FailedCreateSession(true, FailedReason);
		}
		return false;
	case PACKET::ENSF_SUCCEED:
		if (m_GameInstance) {
			m_GameInstance->SetSessionName(ANSI_TO_TCHAR(SessionName.c_str()));
		}
		return true;
	}
	return false;
}

void AHANSEIRacingGameModeBase::DisconnectSucceed(std::stringstream& RecvStream) {
	int32 IsFailed = -1;
	RecvStream >> IsFailed;

	if (IsFailed == EFAILED::EF_SUCCEED) {
		m_bDisconnected = true;
	}
}

void AHANSEIRacingGameModeBase::SendPlayerLoginInformationToServer(const EPACKETMESSAGE& PackType, const FString& ID, const FString& Password) {
	std::stringstream SendStream;

	SendStream << PackType << std::endl;
	SendStream << TCHAR_TO_ANSI(*ID) << std::endl;
	SendStream << TCHAR_TO_ANSI(*Password) << std::endl;

	if (GetClientSocket()) {
		GetClientSocket()->Send(SendStream.str().c_str(), SendStream.str().length());
	}
}

void AHANSEIRacingGameModeBase::SendPlayerSignUpInformationToServer(const EPACKETMESSAGE& PackType, const FString& NickName, const FString& ID, const FString& Password) {
	std::stringstream SendStream;

	SendStream << PackType << std::endl;
	SendStream << TCHAR_TO_ANSI(*NickName) << std::endl;
	SendStream << TCHAR_TO_ANSI(*ID) << std::endl;
	SendStream << TCHAR_TO_ANSI(*Password) << std::endl;

	if (GetClientSocket()) {
		GetClientSocket()->Send(SendStream.str().c_str(), SendStream.str().length());
	}
}

void AHANSEIRacingGameModeBase::SendCreateSessionInformationToServer(const EPACKETMESSAGE & PackType, const FString & SessionName, const int32 MaxPlayer, const bool bUsePassword, const FString & Password) {
	std::stringstream SendStream;

	SendStream << PackType << std::endl;
	SendStream << TCHAR_TO_ANSI(*SessionName) << std::endl;
	SendStream << MaxPlayer << std::endl;
	SendStream << bUsePassword << std::endl;
	SendStream << TCHAR_TO_ANSI(*Password) << std::endl;

	if (GetClientSocket()) {
		GetClientSocket()->Send(SendStream.str().c_str(), SendStream.str().length());
	}
}

void AHANSEIRacingGameModeBase::SendAllSessionInformtaionToServer(const EPACKETMESSAGE& PacketType, const int32 MinLimit) {
	std::stringstream SendStream;
	
	SendStream << PacketType << std::endl << MinLimit << std::endl;

	if (GetClientSocket()) {
		GetClientSocket()->Send(SendStream.str().c_str(), SendStream.str().length());
	}
}

void AHANSEIRacingGameModeBase::SendJoinSessionToServer(const EPACKETMESSAGE& PacketType, const FString & SessionName, const bool bUsePassword, const FString & Password) {
	std::stringstream SendStream;

	SendStream << PacketType << std::endl << TCHAR_TO_ANSI(*SessionName) << std::endl << bUsePassword << std::endl << TCHAR_TO_ANSI(*Password) << std::endl;

	if (GetClientSocket()) {
		GetClientSocket()->Send(SendStream.str().c_str(), SendStream.str().length());
	}
}

void AHANSEIRacingGameModeBase::SendDisconnectToServer(const EPACKETMESSAGE& PacketType) {
	std::stringstream SendStream;

	SendStream << PacketType << std::endl;

	if (GetClientSocket()) {
		GetClientSocket()->Send(SendStream.str().c_str(), SendStream.str().length());
	}
}

void AHANSEIRacingGameModeBase::SetLoginWidgetClass(UUserWidget* Widget) {
	m_LoginWidget = Cast<ULoginWidget>(Widget);
}

void AHANSEIRacingGameModeBase::SetMainScreenWidgetClass(UUserWidget * Widget) {
	m_MainWidget = Cast<UMainScreenWidget>(Widget);
}