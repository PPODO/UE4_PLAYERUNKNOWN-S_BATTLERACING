#include "SocketComponent.h"
#include "UdpSocketBuilder.h"
#include "SocketSubsystem.h"
#include "PlatformProcess.h"
#include "IPAddress.h"
#include "Engine/Engine.h"

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
			m_Socket->Connect(*m_Address);
		}
		else if(m_Socket && m_GameMode) {
			if (m_Socket->HasPendingData(PendingData) && m_Socket->Recv((uint8*)MessageBuffer, MaxBufferSize, RecvBytes)) {
				m_GameMode->RecvDataProcessing(MessageBuffer, RecvBytes);
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

	m_Address = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	m_Address->SetIp(L"192.168.1.57", bIsValid);
	m_Address->SetPort(Port);

	m_Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_None, SocketName, false);
	if (m_Socket) {
		m_Socket->Connect(*m_Address);
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
			m_Address = nullptr;
			m_GameMode = nullptr;
		}
	}
}

FUDPSocketComponent::FUDPSocketComponent() : m_UDPReceiver(nullptr), m_Socket(nullptr) {
}

FUDPSocketComponent::~FUDPSocketComponent() {
	if (m_UDPReceiver) {
		delete m_UDPReceiver;
		m_UDPReceiver = nullptr;
	}

	if (m_Socket) {
		m_Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(m_Socket);
	}
}

bool FUDPSocketComponent::StartUDPReceiver() {
	bool bCanBind = false;
	auto Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBind);

	if (Addr->IsValid()) {
		FIPv4Address Address;
		FIPv4Address::Parse(Addr->ToString(false), Address);
		FIPv4Endpoint EndPoint(Address, 3515);
		FTimespan ThreadWaitTime = FTimespan::FromMilliseconds(100);

		m_Socket = FUdpSocketBuilder(L"Controller Socket").AsNonBlocking().AsReusable().BoundToEndpoint(EndPoint).WithReceiveBufferSize(MaxBufferSize);
		m_UDPReceiver = new FUdpSocketReceiver(m_Socket, ThreadWaitTime, L"UDP Socket Receiver");
		return true;
	}
	return false;
}