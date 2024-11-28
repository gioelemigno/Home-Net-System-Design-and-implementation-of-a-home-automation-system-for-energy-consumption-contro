#pragma once
//#pragma message "HNSSocket.h"

#include "../HNSCore/HNSMacro.h"

/******************** REFRSH ALL *****************************/
/*
* MASTER:  size 0 | type REFRESH_ALL
*
* SLAVE:   size sizeDataRefreshAll | type RSP_ALL 
*        | data chip_temp status0 pwr_limit3 pwr_limit2 pwr_limit1 pwr_limit0 real_pwr3 real_pwr2 real_pwr1 real_pwr0 ...
*                         |------------------sub_socket_i-------------------------------------------------------------|
*
*   NOTE:
*       Each subSokcte can have three compoents:
*           -switch
*           -real power limit (auto swicth)
*           -sensor
*       A device can't have one or more componets, MASTER and SLAVE exchange this information during init()
*
*       Sensor component can be composed by multiple float variable, this depend by subSocketSensorType
*       In this example we consider a only real_power sensor, so only one float variable
*
*/

/********************* REFRESH *****************************/
/*
* MASTER:  size 0 | type REFRESH
*
* SLAVE:   size 1 
*          type RSP_ONLY_TEMP
*          data chipTemperature0
*
* SLAVE:   size 0
*          type RSP_NO_CHANGE
*
* SLAVE:   size x
*          type RSP_REFRESH_n   -> n indicates number of elements in payload
*          data chipTemperature0
*               <wordChanges>
*               ------------
*               subSocketChanges_i
*               -----------
*               ****************
*               <wordChanges>
*               ------------
*               subSocketChanges_j
*               ***************
*
*   wordChanges:
*                   MSb                LSb
*                   h g f x  x SNR ASW SW
*
*                - SW (SWICTH): if set => data contains switch status (1 Byte)
*                - ASW (AUTO_SWITCH): if set => data contains power_limit (4 Byte)
*                - SNR (SENSOR): if set => data contains socket's sensor
*                 
*               - hgf: indicates which subSocket_i
*               
*               -xx: reserved
*
*   NOTE:
*       - sensor data use this priority layout:
*           switch >> powerLimit >> sensor
*          one or more components can't be, but at least one must be
*
*       - sensor data depends by subSocketType
*
*   EXAMPLE SLAVE: it returns only one subSocket (case only realPower type)
*
*   SLAVE:  size 7
*           type RSP_REFRESH_1
*           data 
*               40
*
*               000 xx 1 1 1
*
*               POWER_OFF
* 
*               pwr_limit3
*               pwr_limit2
*               pwr_limit1
*               pwr_limit0
* 
*               real_pwr3
*               real_pwr2
*               real_pwr1
*               real_pwr0
*
*
*
*   SLAVE:  size 7
*           type RSP_REFRESH_1
*           data 
*               42
*
*               000 xx 1 0 1
*
*               POWER_OFF
*
*               real_pwr3
*               real_pwr2
*               real_pwr1
*               real_pwr0
*
*/


/*********************** SET SWICTH STATUS ******************************
*
*   MASTER:    size 1  
*              type STATUS_POWER_ON or STATUS_POWER_OFF
*              data 
*                   wordChanges

*   SLAVE:     size 1 
*              type RSP_SET_STATUS_ON or RSP_SET_STATUS_OFF
*              data 
*                   wordChanges
*
*   wordChanges:
*                   MSb                LSb
*                   h g f x  x SNR ASW SW
*
*                - SW (SWICTH): set status of swicth
*                - ASW (AUTO_SWITCH): set status auto swicth
*                - SNR (SENSOR): set status sensor
*                 
*               - hgf: indicates which subSocket_i
*               
*               -xx: reserved
*/

/* MOVED TO HNSMacro.h
#define STATUS_POWER_ON     STATUS_2
#define STATUS_POWER_OFF    STATUS_3
#define STATUS_POWER_ALLARM STATUS_4
*/


/*
* Following macro is used to cominicate to hub, what quantity device returns
*/
#define REAL_POWER 1
#define APPARENT_POWER (1<<1)
#define I_RMS (1<<2)
#define V_RMS (1<<3)
#define SWITCH (1<<4)
#define AUTO_SWITCH_PWR_LIMIT (1<<5)

//common default setting
#define DEFAULT_POWER_LIMIT 2001.0f

//used to save what changed and slave exchange to Master during REFRESH
// sizeof(subSocketChanges_t is 1 Byte)
typedef struct subSocketChanges_t{
    uint8_t switchStatus : 1;
    uint8_t autoSwitchPwrLimit : 1;
    uint8_t sensors : 1;
    
    uint8_t RESERVED : 2;

    uint8_t indexSubSocket : 3;

}subSocketChanges_t;

