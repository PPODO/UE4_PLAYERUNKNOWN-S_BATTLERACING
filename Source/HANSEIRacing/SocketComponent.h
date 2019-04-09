#pragma once
#define _CRT_SRCURE_NO_WARNINGS

#include "Sockets.h"
#include "CoreMinimal.h"
#include "RunnableThread.h"
#include "UdpSocketReceiver.h"
#include "BaseGameMode.h"
#include "Runnable.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <mutex>

static const int32 MaxBufferSize = 1024;
static const int32 MaxNameLen = 15;

struct FInputMotionData {
public:
	float m_Steering;
	float m_Throttle;
	bool m_HandBreak;

	FInputMotionData() : m_Steering(0.f), m_Throttle(0.f), m_HandBreak(false) {};
	FInputMotionData(float& Steering, float Throttle, bool HandBreak) : m_Steering(Steering), m_Throttle(Throttle), m_HandBreak(HandBreak) {};
};

FORCEINLINE FArchive& operator<<(FArchive &Ar, FInputMotionData& TheStruct) {
	Ar << TheStruct.m_Steering;
	Ar << TheStruct.m_Throttle;
	Ar << TheStruct.m_HandBreak;

	return Ar;
}

class HANSEIRACING_API FUDPSocketComponent {
private:
	FSocket* m_Socket;
	FUdpSocketReceiver* m_UDPReceiver;

public:
	FUDPSocketComponent();
	~FUDPSocketComponent();

public:
	bool StartUDPReceiver();

public:
	inline FUdpSocketReceiver* GetUDPReceiver() const { return m_UDPReceiver; }

};

class HANSEIRACING_API FSocketComponent : public FRunnable {
private:
	FRunnableThread* m_RunnableThread;
	FThreadSafeCounter m_ThreadSafeCounter;
	std::mutex m_Lock;

private:
	FSocket* m_Socket;
	TSharedPtr<FInternetAddr> m_Address;
	uint8 MessageBuffer[MaxBufferSize];

private:
	class ABaseGameMode* m_GameMode;

protected:
	virtual void Stop() override;

public:
	FSocketComponent();
	virtual ~FSocketComponent() override;

public:
	virtual bool Init() override;
	virtual uint32 Run() override;

public:
	void ConnectToServer(class ABaseGameMode* GM, int32 Port, const FString& SocketName);
	void DisconnectSocket();
	void StopThread();
	bool Send(const ANSICHAR* Message, uint32 Length);

	ESocketConnectionState GetConnectionState() const { return m_Socket->GetConnectionState(); }
	FSocket* GetSocket() const { return m_Socket; }

};

class Session {
public:
	int m_SessionID;
	std::string m_SesisonName;
	int m_MaxPlayer;
	bool m_bUsePassword;
	std::string m_Password;
	int m_CurrentPlayer;
	bool m_GameState;

public:
	Session() : m_SessionID(-1), m_SesisonName(""), m_MaxPlayer(0), m_bUsePassword(false), m_Password(""), m_CurrentPlayer(0), m_GameState(false) {};

public:
	friend std::ostream& operator<<(std::ostream& os, Session& Info) {
		os << Info.m_SessionID << std::endl;
		os << Info.m_SesisonName << std::endl;
		os << Info.m_MaxPlayer << std::endl;
		os << Info.m_bUsePassword << std::endl;
		os << Info.m_Password << std::endl;
		os << Info.m_CurrentPlayer << std::endl;
		os << Info.m_GameState << std::endl;

		return os;
	}

	friend std::istream& operator>>(std::istream& is, Session& Info) {
		is >> Info.m_SessionID;
		is >> Info.m_SesisonName;
		is >> Info.m_MaxPlayer;
		is >> Info.m_bUsePassword;
		is >> Info.m_Password;
		is >> Info.m_CurrentPlayer;
		is >> Info.m_GameState;

		return is;
	}
};

class SessionInformation {
public:
	std::vector<Session> m_Sessions;

public:
	friend std::ostream& operator<<(std::ostream& os, SessionInformation& Information) {
		for (auto It : Information.m_Sessions) {
			os << It;
		}

		return os;
	}

	friend std::istream& operator>>(std::istream& is, SessionInformation& Information) {
		size_t Count = 0;
		is >> Count;

		for (size_t i = 0; i < Count; i++) {
			Session NewSession;
			is >> NewSession;
			Information.m_Sessions.push_back(NewSession);
		}
		return is;
	}

};