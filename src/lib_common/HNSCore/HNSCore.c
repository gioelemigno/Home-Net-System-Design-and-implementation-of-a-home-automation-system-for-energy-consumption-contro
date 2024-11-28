#include "HNSCore.h"
#include <stdint.h>
#include <stdlib.h>

#define HNS_MODEL_STRING_SIZE 7

const char* HNSModelString[HNS_MODEL_STRING_SIZE] = {
    "NULL",
    "HNSHub",
	"HNSTemperatureSensor",
	"HNSLight",
	"HNSTemperatureHumiditySensor",
	"HNSSocket",
	"HNSSocketPower"
};
const uint16_t HNSModelString_size = HNS_MODEL_STRING_SIZE;

const char* HNSModel_getModelName(uint16_t model){
	return (model >= HNSModelString_size)? NULL : HNSModelString[model]; 
}