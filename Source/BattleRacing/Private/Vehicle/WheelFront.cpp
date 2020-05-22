#include "Vehicle/WheelFront.h"
#include "UObject/ConstructorHelpers.h"
#include "TireConfig.h"

UWheelFront::UWheelFront() {
	ShapeRadius = 18.f;
	ShapeWidth = 15.f;
	SteerAngle = 60.f;
	bAffectedByHandbrake = false;

	LatStiffMaxLoad = 1.f;
	LatStiffValue = 7.f;
	LongStiffValue = 500.f;

	SuspensionForceOffset = -15.f;
	SuspensionMaxRaise = 8.f;
	SuspensionMaxDrop = 12.f;
	SuspensionNaturalFrequency = 9.f;
	SuspensionDampingRatio = 1.05f;

	::ConstructorHelpers::FObjectFinder<UTireConfig> tireData(L"TireConfig'/Game/Meshes/Vehicle/WheelData/Vehicle_FrontTireConfig.Vehicle_FrontTireConfig'");
	if (tireData.Succeeded()) {
		TireConfig = tireData.Object;
	}
}