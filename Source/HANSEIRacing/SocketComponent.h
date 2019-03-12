#pragma once

#include "Sockets.h"
#include "Runnable.h"
#include "CoreMinimal.h"
#include "RunnableThread.h"
#include <mutex>
#include <iostream>
#include <sstream>
#include <vector>

static const int32 MaxBufferSize = 1024;

class HANSEIRACING_API FSocketComponent : public FRunnable {
private:
	FRunnableThread* m_RunnableThread;
	FThreadSafeCounter m_ThreadSafeCounter;
	std::mutex m_Lock;

private:
	const int32 m_Port;
	FSocket* m_Socket;
	uint8 m_RecvBuffer[MaxBufferSize];

private:
	class ABaseGameMode* m_GameMode;

protected:
	virtual void Stop() override;

public:
	FSocketComponent(class ABaseGameMode* GM, const int32 Port);
	virtual ~FSocketComponent() override;

	virtual bool Init() override;
	virtual uint32 Run() override;

public:
	void StopThread();

public:
	void Send(const ANSICHAR* Message, uint32 Length);

};

namespace PACKET {
	enum EFAILED {
		EF_EXIST,
		EF_FAILED,
		EF_SUCCESSED
	};
}

namespace PLAYER {
	class Character {

	};


}

namespace SESSION {
	enum GAMESTATE { LOBBY, INGAME };

	class Session {
	public:
		std::string m_SessionName;
		int m_MaxPlayer;
		bool m_bUsePassword;
		std::string m_Password;
		int m_CurrentPlayer;
		int m_GameState;

	public:
		Session();
		~Session();

		friend std::ostream& operator<<(std::ostream& os, Session& Info) {
			os << Info.m_SessionName << std::endl;
			os << Info.m_MaxPlayer << std::endl;
			os << Info.m_bUsePassword << std::endl;
			os << Info.m_Password << std::endl;
			os << Info.m_CurrentPlayer << std::endl;
			os << Info.m_GameState << std::endl;

			return os;
		}

		friend std::istream& operator>>(std::istream& is, Session& Info) {
			is >> Info.m_SessionName;
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
		SessionInformation();
		~SessionInformation();

		friend std::ostream& operator<<(std::ostream& os, SessionInformation& Info) {
			for (auto Iterator : Info.m_Sessions) {
				os << Iterator;
			}
			return os;
		}

		friend std::istream& operator>>(std::istream& is, SessionInformation& Info) {
			size_t Size = 0;
			is >> Size;

			for (size_t i = 0; i < Size; i++) {
				Session NewSession;
				is >> NewSession;
				Info.m_Sessions.push_back(NewSession);
			}
			return is;
		}
	};
}