#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UdpSocketComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLERACING_API UUdpSocketComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UUdpSocketComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	bool StartUDPSocket();

private:
	class FSocket* mSocket;
	class FUdpSocketReceiver* mUdpSocketReceiver;
	
};