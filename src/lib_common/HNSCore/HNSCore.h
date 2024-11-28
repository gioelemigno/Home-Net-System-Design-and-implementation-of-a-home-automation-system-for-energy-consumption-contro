#pragma once
#include "HNSError.h"
#include "HNSRouting.h"
#include "HNSMacro.h"
#include <stdint.h>

typedef enum HNSModel {
	HNSModel_HNSHub = 1,
	HNSModel_HNSTemperatureSensor,
	HNSModel_HNSLight,
	HNSModel_HNSTemperatureHumiditySensor,
	HNSModel_HNSSocket,
	HNSModel_HNSSocketPower
}HNSModel;

const char* HNSModel_getModelName(uint16_t model);
//extern const char* HNSModelString[]; 