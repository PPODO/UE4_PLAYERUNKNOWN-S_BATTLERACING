#include "GameModeBase/InGameModeBase.h"
#include "Vehicle/WheeledVehicle4W.h"

AInGameModeBase::AInGameModeBase() {
	DefaultPawnClass = AWheeledVehicle4W::StaticClass();
}