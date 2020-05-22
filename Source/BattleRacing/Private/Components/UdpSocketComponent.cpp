#include "Components/UdpSocketComponent.h"
#include "Common/UdpSocketBuilder.h"
#include "Common/UdpSocketReceiver.h"

UUdpSocketComponent::UUdpSocketComponent() : mSocket(nullptr), mUdpSocketReceiver(nullptr) {

	PrimaryComponentTick.bCanEverTick = true;
}

void UUdpSocketComponent::BeginPlay() {
	Super::BeginPlay();

}

void UUdpSocketComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UUdpSocketComponent::StartUDPSocket() {
	

	return false;
}
