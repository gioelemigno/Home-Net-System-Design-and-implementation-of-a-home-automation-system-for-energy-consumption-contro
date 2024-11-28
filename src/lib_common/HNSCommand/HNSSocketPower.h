#ifndef HNS_SOCKET_H_
#define HNS_SOCKET_H_

//#pragma message "HNSSocket.h"

#include "../HNSCore/HNSMacro.h"
/*
* MASTER:  size 0 | type READ_STATUS_CURRENT
*
* SLAVE:   size 5 | type SEND_STATUS_CURRENT | data status0 current3 current2 current1 current0
*/
    #define READ_STATUS_CURRENT     READ_1
    #define SEND_STATUS_CURRENT     RESPONSE_1

/*
*   MASTE: size 0 | type READ_CURRENT_LIMIT
*
*   SLAVE: size 4 | type SEND_CURRENT_LIMIT | data currentLimit3 currentLimit2 currentLimit1 currentLimit0
*/
    #define READ_CURRENT_LIMIT READ_2
    #define SEND_CURRENT_LIMIT RESPONSE_2

/*  
* MASTER:   size 0 | type SET_STATUS_ON
*           size 0 | type SET_STATUS_OFF
*
* SLAVE:    size 1 | type ACK_SET_STATUS | data status0
*/
    #define SET_STATUS_ON   SET_1
    #define SET_STATUS_OFF  SET_2
    #define ACK_SET_STATUS  ACK_1

/*
*   MASTER: size 4 | type SET_CURRENT_LIMIT | data currenLimit3 currenLimit2 currenLimit1 currenLimit0
*
*   SLAVE:  size 4 | type ACK_SET_CURRENT_LIMIT | data currenLimit3 currenLimit2 currenLimit1 currenLimit0
*/
    #define SET_CURRENT_LIMIT  SET_3
    #define ACK_SET_CURRENT_LIMIT ACK_2

#define STATUS_POWER_ON     STATUS_2
#define STATUS_POWER_OFF    STATUS_3
#define STATUS_POWER_ALLARM STATUS_4

/*
* Following macro is used to cominicate to hub, what quantity device returns
*/
#define REAL_POWER 1
#define APPARENT_POWER (1<<1)
#define I_RMS (1<<2)
#define V_RMS (1<<3)
#define SWITCH (1<<4)
#define POWER_FACTOR (1<<5)

//common default setting
#define DEFAULT_POWER_LIMIT 2000.0f
#endif /*HNS_SOCKET_H_*/
