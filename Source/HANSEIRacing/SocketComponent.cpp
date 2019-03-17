#include "SocketComponent.h"
#include "SocketSubsystem.h"
#include "PlatformProcess.h"
#include "BaseGameMode.h"
#include "IPAddress.h"

FSocketComponent::FSocketComponent() : m_GameMode(nullptr), m_ThreadSafeCounter(0), m_Socket(nullptr), m_Address(nullptr) {
	memset(MessageBuffer, 0, MaxBufferSize);
	m_RunnableThread = FRunnableThread::Create(this, L"SocketThread", 0, TPri_BelowNormal);
}

FSocketComponent::~FSocketComponent() {
	DisconnectSocket();
	if (m_RunnableThread) {
		delete m_RunnableThread;
		m_RunnableThread = nullptr;
	}
}

bool FSocketComponent::Init() {

	return true;
}

uint32 FSocketComponent::Run() {
	FPlatformProcess::Sleep(0.03);
	int32 RecvBytes = 0;
	uint32 PendingData = 0;

	while (m_ThreadSafeCounter.GetValue() == 0) {
		if (m_Socket && m_GameMode && m_Address && m_Socket->GetConnectionState() != ESocketConnectionState::SCS_Connected) {
			std::unique_lock<std::mutex> Lock(m_Lock);
			if (m_Socket->Connect(*m_Address)) {
				m_GameMode->SetConnected(true);
			}
			else {
				m_GameMode->m_bFailedJoinGame = true;
			}
		}
		else if(m_Socket && m_GameMode) {
			if (m_Socket->HasPendingData(PendingData) && m_Socket->Recv((uint8*)MessageBuffer, MaxBufferSize, RecvBytes)) {
				m_GameMode->RecvDataProcessing(UTF8_TO_TCHAR(MessageBuffer));
				memset(MessageBuffer, 0, MaxBufferSize);
			}
		}
	}
	return 0;
}

void FSocketComponent::Stop() {
	m_ThreadSafeCounter.Increment();
}

void FSocketComponent::StopThread() {
	Stop();
}

bool FSocketComponent::Send(const ANSICHAR* Message, uint32 Length) {
	int SendBytes = 0;
	if (m_Socket && m_Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected) {
		return m_Socket->Send((uint8*)Message, Length, SendBytes);
	}
	return false;
}

void FSocketComponent::ConnectToServer(class ABaseGameMode* GM, int32 Port, const FString& SocketName) {
	bool bIsValid = true, bCanBind = false;
	if (m_Socket) {
		m_Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(m_Socket);
	}

	TSharedRef<FInternetAddr> LocalIP = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBind);
	if (LocalIP->IsValid()) {
		m_Address = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		m_Address->SetIp(*LocalIP->ToString(false), bIsValid);
		m_Address->SetPort(Port);
	}

	m_Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_None, SocketName, false);
	if (m_Socket) {
		if (m_Socket->Connect(*m_Address)) {
			GM->SetConnected(true);
		}
		else {
			GM->m_bFailedJoinGame = true;
		}
	}

	if (GM) {
		m_GameMode = GM;
	}
}

void FSocketComponent::DisconnectSocket() {
	std::unique_lock<std::mutex> Lock(m_Lock);
	if (m_Socket && m_Socket->GetConnectionState() == ESocketConnectionState::SCS_Connected) {
		if (m_Socket->Shutdown(ESocketShutdownMode::ReadWrite) && m_Socket->Close()) {
			ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(m_Socket);
			m_GameMode = nullptr;
		}
	}
}

SESSION::Session::Session() {
}

SESSION::Session::~Session() {
}

SESSION::SessionInformation::SessionInformation() {
}

SESSION::SessionInformation::~SessionInformation() {
}

PLAYER::CharacterInformation::CharacterInformation() {
}

PLAYER::CharacterInformation::~CharacterInformation() {
}

PLAYER::Character::Character() {
}

PLAYER::Character::~Character() {
}
