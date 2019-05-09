#include "RearVehicleWheel.h"
#include "ConstructorHelpers.h"
#include "TireConfig.h"

URearVehicleWheel::URearVehicleWheel() {
	ShapeRadius = 18.f;
	ShapeWidth = 15.f;
	bAffectedByHandbrake = true;
	SteerAngle = 0.f;

	LatStiffMaxLoad = 2.25f;
	LatStiffValue = 5.5f;
	LongStiffValue = 1000.f;

	SuspensionForceOffset = -15.f;
	SuspensionMaxRaise = 8.f;
	SuspensionMaxDrop = 12.f;
	SuspensionNaturalFrequency = 9.f;
	SuspensionDampingRatio = 1.05f;

	ConstructorHelpers::FObjectFinder<UTireConfig> TireData(L"TireConfig'/Game/Vehicle/WheelData/Vehicle_BackTireConfig.Vehicle_BackTireConfig'");
	if (TireData.Succeeded()) {
		TireConfig = TireData.Object;
	}
}