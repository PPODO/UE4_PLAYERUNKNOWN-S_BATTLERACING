#pragma once

#include "Sockets.h"
#include "CoreMinimal.h"
#include "RunnableThread.h"
#include "Runnable.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <mutex>

static const int32 MaxBufferSize = 1024;

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

};

namespace PACKET {
	enum EJOINFAILED {
		EJF_NONE = -1,
		EJF_FAILED,
		EJF_INVALIDSESSION,
		EJF_WRONGPASS,
		EJF_MAXPLAYER,
		EJF_SUCCEED
	};

	enum EFAILED {
		EF_FAILED,
		EF_EXIST,
		EF_SUCCEED
	};

	enum ELOGINFAILED {
		ELF_WRONGID,
		ELF_WRONGPASS,
		ELF_FAILED,
		ELF_SUCCEED
	};

	enum ESIGNUPFAILED {
		ESF_EXISTNICKNAME,
		ESF_EXISTID,
		ESF_FAILED,
		ESF_SUCCEED
	};

	enum ENEWSESSIONFAILED {
		ENSF_FAILED,
		ENSF_EXIST,
		ENSF_SUCCEED
	};
}

namespace GAMEPACKET {
	enum EJOINSTATE {
		EJS_FAILED,
		EJS_SUCCEDD
	};
}

namespace PLAYER {
	struct Vector {
		float X, Y, Z;

	public:
		Vector() { X = 0, Y = 0, Z = 0; };
		Vector(float _X, float _Y, float _Z) : X(_X), Y(_Y), Z(_Z) {};

		friend std::ostream& operator<<(std::ostream& os, Vector& Vec) {
			os << Vec.X << std::endl;
			os << Vec.Y << std::endl;
			os << Vec.Z << std::endl;

			return os;
		}

		friend std::istream& operator>>(std::istream& is, Vector& Vec) {
			is >> Vec.X;
			is >> Vec.Y;
			is >> Vec.Z;

			return is;
		}

		inline bool operator==(const FVector& Vec2) {
			if (FMath::IsNearlyEqual(this->X, Vec2.X) && FMath::IsNearlyEqual(this->Y, Vec2.Y) && FMath::IsNearlyEqual(this->Z, Vec2.Z)) {
				return true;
			}
			return false;
		}

		inline bool operator!=(const FVector& Vec2) {
			if (!FMath::IsNearlyEqual(this->X, Vec2.X) || !FMath::IsNearlyEqual(this->Y, Vec2.Y) || !FMath::IsNearlyEqual(this->Z, Vec2.Z)) {
				return true;
			}
			return false;
		}

		inline PLAYER::Vector& operator=(const FVector& Vec2) {
			this->X = Vec2.X;
			this->Y = Vec2.Y;
			this->Z = Vec2.Z;

			return *this;
		}
	};

	class Character {
	public:
		unsigned int m_UniqueKey;
		std::string m_PlayerName;
		Vector m_Location;
		Vector m_Rotation;

	public:
		Character();
		~Character();

		friend std::ostream& operator<<(std::ostream& os, Character& Info) {
			os << Info.m_UniqueKey << std::endl;
			os << Info.m_PlayerName << std::endl;
			os << Info.m_Location << std::endl;
			os << Info.m_Rotation << std::endl;

			return os;
		}

		friend std::istream& operator>>(std::istream& is, Character& Info) {
			is >> Info.m_UniqueKey;
			is >> Info.m_PlayerName;
			is >> Info.m_Location;
			is >> Info.m_Rotation;

			return is;
		}

		void operator=(const Character& Char) {
			this->m_Location = Char.m_Location;
			this->m_PlayerName = Char.m_PlayerName;
			this->m_Rotation = Char.m_Rotation;
			this->m_UniqueKey = Char.m_UniqueKey;
		}
	};

	class CharacterInformation {
	public:
		std::vector<Character> m_Characters;

	public:
		CharacterInformation();
		~CharacterInformation();

		friend std::ostream& operator<<(std::ostream& os, CharacterInformation& Info) {
			for (auto Iterator : Info.m_Characters) {
				os << Iterator << std::endl;
			}
			return os;
		}

		friend std::istream& operator>>(std::istream& is, CharacterInformation& Info) {
			int32 Size = -1;
			is >> Size;

			for (int32 i = 0; i < Size; i++) {
				Character NewCharacter;
				is >> NewCharacter;

				Info.m_Characters.push_back(NewCharacter);
			}
			return is;
		}
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