#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "CoreMinimal.h"
#include "BaseGameMode.h"
#include <vector>
#include <stack>
#include "InGameMode.generated.h"

static const int32 NickNameMaxLen = 15;

UENUM(BlueprintType)
enum class EPACKETMESSAGEFORGAMETYPE : uint8 {
	EPMGT_JOIN,
	EPMGT_UPDATE,
	EPMGT_DISCONNECT,
	EPMGT_STARTGAME,
	EPMGT_SPAWNITEM,
	EPMGT_NEWPLAYER,
	EPMGT_DISCONNECTOTHER,
	EPMGT_COUNT
};

enum class EPACKETTYPE : uint8 {
	EPT_PLAYER,
	EPT_SPAWNER,
	EPT_COUNT
};

enum class EITEMTYPE : uint8 {
	EIT_HEAL,
	EIT_COUNT
};

struct VECTOR {
	float x, y, z;

public:
	VECTOR() : x(0.f), y(0.f), z(0.f) {};

	void operator=(const FVector& Vec) {
		this->x = Vec.X;
		this->y = Vec.Y;
		this->z = Vec.Z;
	}

	void operator=(const FRotator& Rota) {
		this->x = Rota.Pitch;
		this->y = Rota.Yaw;
		this->z = Rota.Roll;
	}
};

struct PACKET {
	EPACKETTYPE m_PacketType;
	EPACKETMESSAGEFORGAMETYPE m_MessageType;
	EPACKETFAILEDTYPE m_FailedReason;
	UINT_PTR m_Socket;

public:
	PACKET() : m_PacketType(EPACKETTYPE::EPT_COUNT), m_MessageType(EPACKETMESSAGEFORGAMETYPE::EPMGT_COUNT), m_FailedReason(EPACKETFAILEDTYPE::EPFT_COUNT) {};

};

struct ITEM {
	EITEMTYPE m_ItemType;
	bool m_bIsActivated;
	size_t m_SpawnerID;
	size_t m_Index;

public:
	ITEM() : m_ItemType(EITEMTYPE::EIT_COUNT), m_bIsActivated(false), m_SpawnerID(0), m_Index(0) {};

};

struct GAMEPACKET : public PACKET {
	int m_SessionID;
	int m_UniqueKey;
	VECTOR m_Location;
	VECTOR m_Rotation;
	struct FInputMotionData m_VehicleData;
	ITEM m_ItemInformation;
	char m_PlayerNickName[NickNameMaxLen];

public:
	void operator=(const GAMEPACKET& Data) {
		this->m_UniqueKey = Data.m_UniqueKey;
		this->m_Location = Data.m_Location;
		this->m_Rotation = Data.m_Rotation;
		this->m_VehicleData = Data.m_VehicleData;
		this->m_ItemInformation = Data.m_ItemInformation;
	}

};

struct SPAWNERPACKET : public PACKET {
	ITEM m_ItemInformation;
};

UCLASS()
class HANSEIRACING_API AInGameMode : public ABaseGameMode {
	GENERATED_BODY()

public:
	AInGameMode();

public:
	virtual void RecvDataProcessing(uint8* RecvBuffer, int32& RecvBytes) override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginDestroy() override;

private:
	class UInGameWidget* m_InGameWidget;
	class UHANSEIRacingGameInstance* m_GameInstance;
	class ADefaultVehicleCharacter* m_Character;
	TArray<class AActor*> m_SpawnPoint;
	TArray<class AActor*> m_StartPoint;
	TArray<class AActor*> m_ItemSpawners;
	TMap<int32, class ADefaultVehicleCharacter*> m_CharacterClass;
	bool m_bIsSpawnPlayer;
	UINT_PTR m_SocketNumber;

private:
	bool m_bIsInGame, m_bIsTeleport;

private:
	std::vector<GAMEPACKET> m_PlayerList;
	std::stack<PACKET*> m_PacketStack;

private:
	uint8* RecvBufferShiftProcess(uint8* RecvBuffer, const int32& PacketSize, const int32& CurrentCount);
	void UpdatePlayerLocationAndRotation();
	void SpawnCharacter();
	void TeleportCharacters();
	void ResetPlayerCurrentRank(const int32& UniqueKey, const int32& TargetRank);

private:
	// INLINE Function
	FORCEINLINE int32 CalculatePacketSize(const PACKET* Packet);
	FORCEINLINE AActor* FindSpawnPointByUniqueKey(const int32& UniqueKey);
	FORCEINLINE void SpawnPawnAndAddCharacterList(class ADefaultVehicleCharacter* NewPawn, const int32& UniqueKey, const ANSICHAR* PlayerName, const int32& CurrentIndex);

public:
	// FROM Server
	void IsSucceedJoinGame(GAMEPACKET& Packet);
	void IsSucceedJoinGameNewPlayer(GAMEPACKET& Packet);
	void IsSucceedDisconnectOtherPlayer(GAMEPACKET& Packet);
	void IsSucceedUpdatePlayerInformation(GAMEPACKET& Packet);
	void IsSucceedStartGame(GAMEPACKET& Packet);
	void IsSucceedRespawnItem(SPAWNERPACKET& Packet);

public:
	// TO Server
	void SendJoinGameToServer();
	void SendCharacterInformationToServer(const FVector& Location, const FRotator& Rotation, const struct FInputMotionData& Data);
	void SendDisconnectToServer();
	void SendStartGame();
	void SendRespawnItemToServer(const int32& SpawnerID, const int32& ItemIndex);

public:
	UFUNCTION(BlueprintCallable)
		void SetInGameWidgetClass(class UInGameWidget* Widget) { if (Widget) { m_InGameWidget = Widget; } }

};