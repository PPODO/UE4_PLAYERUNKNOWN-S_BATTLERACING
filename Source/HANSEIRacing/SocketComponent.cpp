#include "SocketComponent.h"
#include "PlatformProcess.h"
#include "SocketSubsystem.h"
#include "BaseGameMode.h"
#include "IPv4Address.h"
#include "IPAddress.h"

FSocketComponent::FSocketComponent(class ABaseGameMode* GM, const int32 Port) : m_GameMode(GM), m_Port(Port), m_ThreadSafeCounter(0) {
	m_RunnableThread = FRunnableThread::Create(this, L"SocketThread", 0, TPri_Normal);
}

FSocketComponent::~FSocketComponent() {
	if (m_Socket) {
		m_Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(m_Socket);
		m_Socket = nullptr;
	}
	if (m_RunnableThread) {
		delete m_RunnableThread;
		m_RunnableThread = nullptr;
	}
}

bool FSocketComponent::Init() {
	bool bCanBind = false;
	TSharedRef<FInternetAddr> LocalIP = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBind);
	
	if (LocalIP->IsValid()) {
		bool bIsValid = false;
		TSharedRef<FInternetAddr> Address = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		Address->SetIp(*LocalIP->ToString(false), bIsValid);
		Address->SetPort(m_Port);
		m_Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, L"InGameSocket", false);

		// m_Socket->WaitForPendingConnection()·Î ¼öÁ¤
		while (m_Socket->GetConnectionState() != ESocketConnectionState::SCS_Connected) {
			m_Socket->Connect(*Address);
		}
	}
	return true;
}

uint32 FSocketComponent::Run() {
	FPlatformProcess::Sleep(0.03);
	int32 RecvBytes = 0;

	while (m_ThreadSafeCounter.GetValue() == 0) {
		m_Socket->Recv((uint8*)m_RecvBuffer, MaxBufferSize, RecvBytes);

		std::unique_lock<std::mutex> Lock(m_Lock);
		if (m_GameMode) {
			m_GameMode->RecvDataProcessing(UTF8_TO_TCHAR(m_RecvBuffer));
			memset(m_RecvBuffer, 0, MaxBufferSize);
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

void FSocketComponent::Send(const ANSICHAR* Message, uint32 Length) {
	int32 SendBytes = 0;
	m_Socket->Send((uint8*)Message, Length, SendBytes);
}

SESSION::Session::Session() {
}

SESSION::Session::~Session() {
}

SESSION::SessionInformation::SessionInformation() {
}

SESSION::SessionInformation::~SessionInformation() {
}
