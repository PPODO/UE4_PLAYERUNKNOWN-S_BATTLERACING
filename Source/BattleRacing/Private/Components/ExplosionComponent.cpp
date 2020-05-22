#include "Components/ExplosionComponent.h"

UExplosionComponent::UExplosionComponent() {

	PrimaryComponentTick.bCanEverTick = true;
}

void UExplosionComponent::BeginPlay() {
	Super::BeginPlay();
	
}

void UExplosionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}