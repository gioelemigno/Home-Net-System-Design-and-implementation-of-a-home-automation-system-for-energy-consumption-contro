
//#if defined(__AVR_ATmega328P__)


#include "../lib/HNSArduino/Arduino.h"
#include "../lib/HNSDevice/HNSDevice.h"

#include <util/delay.h>
#include <avr/io.h>
#include "../lib/util/Atmega328P/Time_Atmega328.h"
#include "../lib/util/Atmega328P/avrMacro.h"

#include <stdio.h>
#include <stdint.h>


#include "../lib/HNSDeviceLib/HNSSocketPower/HNSSocketPower.h"

#include "../lib/HNSCore/HNSMacro.h"


#define SOCKET


#ifdef SOCKET

#define __SOCKET_0	0xAF00
#define __SOCKET_1	0xAF01
#define __SOCKET_2	0xAF02

#define DEFAULT_HNS_UID		__SOCKET_1
HNSModel myModel = HNSModel_HNSSocketPower;


#ifdef UID
	uint16_t hns_UID = DEFAULT_HNS_UID;
#else
	uint16_t hns_UID = DEFAULT_HNS_UID;
	#warning("Using DEFAULT UID! Add C_FLAGS: -DUID=0xFFF2")
#endif /* UID */

void (*netHandler)(void)=HNSSocketPower_netHandler;


void _setup(){
	//HNSDevice_setHNSModel()
	HNSSocketPower_begin();
}

void routine(){
	HNSSocketPower_routine();
}

#endif



//#endif /*__AVR_ATmega328P__*/
