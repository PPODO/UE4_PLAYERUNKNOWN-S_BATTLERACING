#include "Vehicle/WheelRear.h"
#include "UObject/ConstructorHelpers.h"
#include "TireConfig.h"

UWheelRear::UWheelRear() {
	ShapeRadius = 18.f;
	ShapeWidth = 15.f;
	SteerAngle = 0.f;
	bAffectedByHandbrake = true;

	LatStiffMaxLoad = 1.f;
	LatStiffValue = 7.f;
	LongStiffValue = 500.f;

	SuspensionForceOffset = -15.f;
	SuspensionMaxRaise = 8.f;
	SuspensionMaxDrop = 12.f;
	SuspensionNaturalFrequency = 9.f;
	SuspensionDampingRatio = 1.05f;

	::ConstructorHelpers::FObjectFinder<UTireConfig> tireData(L"TireConfig'/Game/Meshes/Vehicle/WheelData/Vehicle_BackTireConfig.Vehicle_BackTireConfig'");
	if (tireData.Succeeded()) {
		TireConfig = tireData.Object;
	}
}