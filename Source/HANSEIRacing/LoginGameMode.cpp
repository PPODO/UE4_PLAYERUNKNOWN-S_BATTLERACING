#include "LoginGameMode.h"
#include "LoginWidget.h"
#include "MainMenuWidget.h"
#include "HANSEIRacingGameInstance.h"
#include "Kismet/GameplayStatics.h"

ALoginGameMode::ALoginGameMode() : m_GameInstance(nullptr), m_LoginWidget(nullptr), m_MenuWidget(nullptr), m_bIsDisConnect(false) {
	m_Port = 3515;
	m_SocketName = L"Login_Socket";

	PrimaryActorTick.bCanEverTick = true;
}

void ALoginGameMode::BeginPlay() {
	ABaseGameMode::BeginPlay();

	m_GameInstance = Cast<UHANSEIRacingGameInstance>(GetGameInstance());
	if (!m_GameInstance) {
		m_GameInstance = Cast<UHANSEIRacingGameInstance>(GetGameInstance());
	}
}

void ALoginGameMode::Tick(float DeltaTime) {
	ABaseGameMode::Tick(DeltaTime);

	if (IsInGameThread()) {
		if (m_bIsDisConnect) {
			UGameplayStatics::OpenLevel(this, "/Game/Maps/sex");
		}
	}
}

void ALoginGameMode::BeginDestroy() {
	ABaseGameMode::BeginDestroy();

}

void ALoginGameMode::RecvDataProcessing(uint8 * RecvBuffer) {
	int32 PacketType = -1;
	std::stringstream RecvStream(TCHAR_TO_ANSI(UTF8_TO_TCHAR(RecvBuffer)));
	RecvStream >> PacketType;

	if (PacketType == static_cast<int32>(EPACKETMESSAGEFORLOGINTYPE::EPMLT_LOGIN)) {
		IsSucceedLogin(RecvStream);
	}
	else if (PacketType == static_cast<int32>(EPACKETMESSAGEFORLOGINTYPE::EPMLT_NEWACCOUNT)) {
		IsSucceedCreateAccount(RecvStream);
	}
	else if (PacketType == static_cast<int32>(EPACKETMESSAGEFORLOGINTYPE::EPMLT_CREATESESSION)) {
		IsSucceedCreateSession(RecvStream);
	}
	else if (PacketType == static_cast<int32>(EPACKETMESSAGEFORLOGINTYPE::EPMLT_JOINSESSION)) {
		IsSucceedJoinSession(RecvStream);
	}
	else if (PacketType == static_cast<int32>(EPACKETMESSAGEFORLOGINTYPE::EPMLT_GETSESSION)) {
		IsSucceedGetSessionList(RecvStream);
	}
}

// FROM Server

void ALoginGameMode::IsSucceedLogin(std::stringstream& RecvStream) {
	int32 Result = -1;
	RecvStream >> Result;

	if (Result >= 0 && Result < static_cast<int32>(EPACKETFAILEDTYPE::EPFT_COUNT)) {
		if (Result == static_cast<int32>(EPACKETFAILEDTYPE::EPFT_SUCCEED)) {
			std::string UserName;
			RecvStream >> UserName;
			if (m_GameInstance) {
				m_GameInstance->SetPlayerNickName(ANSI_TO_TCHAR(UserName.c_str()));
				m_GameInstance->SetIsLogined(true);
			}
		}
		else {
			if (m_LoginWidget) {
				m_LoginWidget->SetPopUp(true, static_cast<EPACKETFAILEDTYPE>(Result));
			}
		}
	}
}

void ALoginGameMode::IsSucceedCreateAccount(std::stringstream& RecvStream) {
	int32 Result = -1;
	RecvStream >> Result;

	if (Result >= 0 && Result < static_cast<int32>(EPACKETFAILEDTYPE::EPFT_COUNT)) {
		if (Result == static_cast<int32>(EPACKETFAILEDTYPE::EPFT_SUCCEED)) {
			std::string UserName;
			RecvStream >> UserName;
			if (m_GameInstance) {
				m_GameInstance->SetPlayerNickName(ANSI_TO_TCHAR(UserName.c_str()));
				m_GameInstance->SetIsLogined(true);
			}
		}
		else {
			if (m_LoginWidget) {
				m_LoginWidget->SetPopUp(true, static_cast<EPACKETFAILEDTYPE>(Result));
			}
		}
	}
}

void ALoginGameMode::IsSucceedCreateSession(std::stringstream & RecvStream) {
	int32 IsSucceed = -1;
	RecvStream >> IsSucceed;

	if (IsSucceed >= 0 && IsSucceed < static_cast<int32>(EPACKETFAILEDTYPE::EPFT_COUNT)) {
		if (IsSucceed == static_cast<int32>(EPACKETFAILEDTYPE::EPFT_SUCCEED)) {
			m_bIsDisConnect = true;
		}
		else {
			if (m_MenuWidget) {
				m_MenuWidget->SetPopup(true, static_cast<EPACKETFAILEDTYPE>(IsSucceed));
			}
		}
	}
}

void ALoginGameMode::IsSucceedGetSessionList(std::stringstream & RecvStream) {
	int32 IsSucceed = -1, SessionCount = 0;
	SessionInformation List;
	RecvStream >> IsSucceed >> SessionCount >> List;

	if (IsSucceed >= 0 && IsSucceed < static_cast<int32>(EPACKETFAILEDTYPE::EPFT_COUNT)) {
		if (IsSucceed == static_cast<int32>(EPACKETFAILEDTYPE::EPFT_SUCCEED)) {
			if (m_MenuWidget) {
				m_MenuWidget->m_MaxPage = (SessionCount - 1) / 8;
				m_MenuWidget->SetSessionList(List);
			}
		}
	}
}

void ALoginGameMode::IsSucceedJoinSession(std::stringstream & RecvStream) {
	int32 IsSucceed = -1;
	RecvStream >> IsSucceed;

	if (IsSucceed >= 0 && IsSucceed < static_cast<int32>(EPACKETFAILEDTYPE::EPFT_COUNT)) {
		if (IsSucceed == static_cast<int32>(EPACKETFAILEDTYPE::EPFT_SUCCEED)) {
			m_bIsDisConnect = true;
		}
		else {
			if (m_MenuWidget) {
				m_MenuWidget->SetPopup(true, static_cast<EPACKETFAILEDTYPE>(IsSucceed));
			}
		}
	}
}

// TO Server

void ALoginGameMode::SendLoginToServer(const FString& ID, const FString& Password) {
	std::stringstream SendStream;
	if (GetSocket() && ID.Len() > 0 && Password.Len() > 0) {
		SendStream << static_cast<int32>(EPACKETMESSAGEFORLOGINTYPE::EPMLT_LOGIN) << std::endl << TCHAR_TO_ANSI(*ID) << std::endl << TCHAR_TO_ANSI(*Password) << std::endl;

		GetSocket()->Send(SendStream.str().c_str(), SendStream.str().length());
	}
}

void ALoginGameMode::SendCreateAccountToServer(const FString & UserName, const FString & ID, const FString & Password) {
	std::stringstream SendStream;
	if (GetSocket() && UserName.Len() > 0 && ID.Len() > 0 && Password.Len() > 0) {
		SendStream << static_cast<int32>(EPACKETMESSAGEFORLOGINTYPE::EPMLT_NEWACCOUNT) << std::endl << TCHAR_TO_ANSI(*UserName) << std::endl << TCHAR_TO_ANSI(*ID) << std::endl << TCHAR_TO_ANSI(*Password) << std::endl;

		GetSocket()->Send(SendStream.str().c_str(), SendStream.str().length());
	}
}

void ALoginGameMode::SendCreateSessionToServer(const FString & SessionName, const int32 & MaxPlayer, const bool & UsePassword, const FString & Password) {
	std::stringstream SendStream;
	if (GetSocket() && SessionName.Len() > 0 && (UsePassword ? Password.Len() > 0 : true) && MaxPlayer > 0) {
		SendStream << static_cast<int32>(EPACKETMESSAGEFORLOGINTYPE::EPMLT_CREATESESSION) << std::endl << TCHAR_TO_ANSI(*SessionName) << std::endl << MaxPlayer << std::endl << UsePassword << std::endl << TCHAR_TO_ANSI(*Password) << std::endl;

		GetSocket()->Send(SendStream.str().c_str(), SendStream.str().length());
	}
}

void ALoginGameMode::SendGetSessionInformationToServer(const int32& CurrentPage) {
	std::stringstream SendStream;

	if (GetSocket() && m_MenuWidget) {
		SendStream << static_cast<int32>(EPACKETMESSAGEFORLOGINTYPE::EPMLT_GETSESSION) << std::endl << (CurrentPage * 8) << std::endl;

		GetSocket()->Send(SendStream.str().c_str(), SendStream.str().length());
	}
}

void ALoginGameMode::SendJoinSessionToServer(const FString & SessionName, const bool & UsePassword, const FString & Password) {
	std::stringstream SendStream;

	if (GetSocket()) {
		SendStream << static_cast<int32>(EPACKETMESSAGEFORLOGINTYPE::EPMLT_JOINSESSION) << std::endl << TCHAR_TO_ANSI(*SessionName) << std::endl << UsePassword << std::endl << TCHAR_TO_ANSI(*Password) << std::endl;

		GetSocket()->Send(SendStream.str().c_str(), SendStream.str().length());
	}
}