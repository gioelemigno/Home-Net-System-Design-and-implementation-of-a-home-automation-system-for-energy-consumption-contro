#pragma once

#include <stdint.h>
#include "../../HNSCore/HNSError.h"

res_t HNSSocketPower_begin();

//res_t HNSSocketCurrent_init();

void HNSSocketPower_netHandler(void);

res_t HNSSocketPower_routine();

// return status
uint8_t HNSSocketPower_getStatus();
float HNSSocketPower_getPowerLimit();
float HNSSocketPower_getPower();


res_t HNSSocketPower_setStatus(uint8_t status);
res_t HNSSocketPower_setPowerLimit(float powerLimit);
