
#include "HNSSocketPower.h"
#include "../../HNSCommand/HNSSocketV2.h"
#include <avr/io.h>
#include "../../util/Atmega328P/avrMacro.h"
#include "../../HNSArduino/Arduino.h"
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../../util/Atmega328P/USART_Atmega328.h"

#include "EmonLib/EmonLib.h"
#include "../../HNSDevice/Adafruit_NeoPixel-master/Adafruit_NeoPixel.h"

#define INDEX_SUB_SOCKET 0

#define RELAY_1 11
#define RELAY_2 12
#define RELAY_ON    LOW
#define RELAY_OFF   HIGH

#define BUTTON  2
#define PRESS_TIME_ms   100  

#define TIMEOUT_ALLARM_ms   5000 //after that, it set status to STATUS_POWER_OFF

#define PIN_VOLT    A0
#define PIN_CURR    A1

#define CALIB_CURR             30.0f
#define CALIB_VOLTAGE         236.5f
#define CALIB_PHASE_SHIFT       1.7f

#define TIMEOUT_EMONLIB     2000
#define CROSSING_EMON_LIB   12//20

#define THRESOLD_IRMS_EMON_LIB  0.10f//0.10f//0.32f//0.10f

/************************ LED ******************************************************/
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      1
#define INTERNAL_LED 6
//RGB color:
#define RED 255,0,0
#define GREEN 0,255,0
#define BLUE 0,0,255

#define YELLOW 255,255,0
#define VIOLET 102,0,204
#define WHITE 255,255,255
#define ORANGE 255,128,0
#define PINK 255,0,255
#define MAGENTA 255,0,127

Adafruit_NeoPixel LedSocketPwr = Adafruit_NeoPixel(NUMPIXELS, INTERNAL_LED, NEO_GRB + NEO_KHZ800);
#define COLOR_OFF       GREEN
#define COLOR_ON        BLUE
#define COLOR_ALLARM    RED

#define BLINK_PERIOD_ms 500
/**************************************************************************************/

EnergyMonitor emon0;                 

//indicates features of socket
uint8_t socketSetting = SWITCH | AUTO_SWITCH_PWR_LIMIT | REAL_POWER;

volatile bool sendData = false;
volatile uint8_t chipTemperature = 0;

volatile uint8_t status_switch = STATUS_POWER_OFF;
volatile float realPower = 0.0;
volatile float power_limit = DEFAULT_POWER_LIMIT;

volatile bool chipTempChange=false;
volatile subSocketChanges_t changes = {
    .switchStatus=0,
    .autoSwitchPwrLimit=0,
    .sensors=0,
    .RESERVED=0,
    .indexSubSocket=INDEX_SUB_SOCKET
};

//function pointer, contains pointer of current routine function
typedef res_t(*routine)(void);

res_t routineStatusON(void);
res_t routineStatusOFF(void);
res_t routineStatusALLARM(void);

unsigned long timetBlinkAllarm;
bool blinkAllarmOn = false;
unsigned long timeStartAllarm;


routine currentStatusRoutine = routineStatusOFF;


// TODO
static inline void readChipTemp();

static inline bool pressButton(uint8_t time_press_multipler);

// set switch status and logical status (i.e. currentStatusRoutine function pointer)
// status can be : STATUS_POWER_ON, STATUS_POWER_OFF, STATUS_POWER_ALARM
static inline void setStatus(uint8_t status);

static inline void writeFloat(uint8_t* dst, volatile float* toWrite);
static inline float readFloat(uint8_t* src);

res_t HNSSocketPower_begin(){

    //setup relay
    pinMode(RELAY_1, OUTPUT);
    pinMode(RELAY_2, OUTPUT);
    digitalWrite(RELAY_1, RELAY_OFF);
    digitalWrite(RELAY_2, RELAY_OFF);

    //setup button
    pinMode(BUTTON, INPUT);
    digitalWrite(BUTTON, HIGH); //PULL-UP

    //setup led
    LedSocketPwr.begin();
    LedSocketPwr.setPixelColor(0, LedSocketPwr.Color(0,0,0));
    LedSocketPwr.show();   


    //setupe EmonLib
    emon0.voltage(PIN_VOLT, CALIB_VOLTAGE, CALIB_PHASE_SHIFT);  // Voltage: input pin, calibration, phase_shift
    emon0.current(PIN_CURR, CALIB_CURR);       // Current: input pin, calibration.

    // EmonLib algorithm adjustment
    // take average 2 seconds
        disable_RX_interrupt();
        disable_packet_ready_interrupt();

    for(int i = 0; i < 100 ; i++){
        emon0.calcVI(20,2000); 
    }
        enable_RX_interrupt();
        enable_packet_ready_interrupt();

    LedSocketPwr.setPixelColor(0, LedSocketPwr.Color(COLOR_OFF));
    LedSocketPwr.show();   
}

/*
* MASTER:  size 0 | type REFRESH_ALL
*
* SLAVE:   size sizeDataRefreshAll | type RSP_ALL 
*        | data chip_temp status0 pwr_limit3 pwr_limit2 pwr_limit1 pwr_limit0 real_pwr3 real_pwr2 real_pwr1 real_pwr0 ...
*                         |------------------sub_socket_i-------------------------------------------------------------|
*/
/*********************** SET SWICTH STATUS ******************************
*
*   MASTER:    size 1  
*              type SET_STATUS_POWER_ON or SET_STATUS_POWER_OFF
*              data 
*                   wordChanges

*   SLAVE:     size 1 
*              type RSP_SET_STATUS_ON or RSP_SET_STATUS_OFF
*              data 
*                   wordChanges
*************************************************************************/
void HNSSocketPower_netHandler(void){
    packet_t packet;
    
    res_t res = HNSRouting_read(&packet);
    if(res == -1)return;

    //this is work beacuse subID is always 0 !!!
    //(only one subSocket). Modify this in future!
   // uint8_t oldChanges = *((volatile uint8_t*)&changes);
    //uint8_t relativeChanges;
     
     //float p = 123.34;

    uint8_t response_type = 0;
    switch (res){
        case 0:
            switch (packet.type){
                case REFRESH_ALL:
                    packet.recipient=HUB_ADDRESS;
                    packet.size= sizeof(chipTemperature) + sizeof(status_switch)  + sizeof(power_limit) + sizeof(realPower);
                    packet.type=RSP_REFRESH_ALL;

                    packet.data[0]=chipTemperature;
                    packet.data[1]=status_switch;
                    
                    writeFloat(&(packet.data[2]), &power_limit);
                    writeFloat(&(packet.data[2+sizeof(power_limit)]), &realPower);
                   
                    //writeFloat(&(packet.data[2+sizeof(power_limit)]), &p);

                    res=HNSRouting_write(&packet);
                    if(res==-1) return;   

                    //sendData=true;
                break;
                
                default:
                break;
            }
        break;
        
        case 1:
            if(packet.type == SET_STATUS_ON){
                //this is work beacuse subID is always 0 !!!
                // modify this in future!
                setStatus(STATUS_POWER_ON);
                response_type = RSP_SET_STATUS_ON;

                //LedSocketPwr.setPixelColor(0, LedSocketPwr.Color(RED));

            }
            else if(packet.type == SET_STATUS_OFF){
               //digitalWrite(13, HIGH);
                //this is work beacuse subID is always 0 !!!
                // modify this in future!
                setStatus(STATUS_POWER_OFF);
                response_type = RSP_SET_STATUS_OFF;
            }
            else {
                return; // error
            }

           // relativeChanges = *((volatile uint8_t*)&changes) ^ oldChanges;

            packet.recipient=HUB_ADDRESS;
            packet.size=1;
            packet.type=response_type;

            // NOT WORKS IN GENERAL CASE !
            packet.data[0]=1;//relativeChanges;
            
            res=HNSRouting_write(&packet);
            if(res==-1) return;   

        break;

        default:
        break;
    }
    
    sendData=true;
                    //setStatus(STATUS_POWER_ON);

}

res_t HNSSocketPower_routine(){
    return currentStatusRoutine();
}

// return status
uint8_t HNSSocketPower_getStatus(){
    return status_switch;
}

float HNSSocketPower_getPowerLimit(){
}    

float HNSSocketPower_getPower(){
    return realPower;
}

res_t HNSSocketPower_setStatus(uint8_t status){
    setStatus(status);
    return 0;
}

res_t HNSSocketPower_setPowerLimit(float powerLimit){
    return -1;
}


res_t routineStatusON(void){
    readChipTemp();
    if(pressButton(1))   {
        setStatus(STATUS_POWER_OFF);
        while(pressButton(1));
        return 0;
    }
    //realPower = 134.993;
    float realPower_readed = 123.4;// = 17.98;
    disable_RX_interrupt();
    disable_packet_ready_interrupt();

    //delay(100);
    emon0.calcVI(CROSSING_EMON_LIB, TIMEOUT_EMONLIB);
    
    //float realPower_readed; //;= realPower;// emon0.realPower;
    realPower_readed =  emon0.realPower;
    
    if(emon0.Irms <= THRESOLD_IRMS_EMON_LIB){
        realPower_readed=0.0f;
    }
    
    if(realPower_readed != realPower){
        realPower = realPower_readed;
        changes.sensors=true;
    }

    enable_RX_interrupt();
    enable_packet_ready_interrupt();
    unsigned long del = millis();

    while(millis() - del < 300 && !sendData);
    
    if(sendData) sendData = false;
    return 0;
}

res_t routineStatusOFF(void){
    readChipTemp();
    if(pressButton(1))   {
        setStatus(STATUS_POWER_ON);
        while(pressButton(1));
        return 0;
    }
    return 0;
}

res_t routineStatusALLARM(void){
    readChipTemp();
    if(pressButton(5)){
        setStatus(STATUS_POWER_ON);
        while(pressButton(1));
        return 0;
    }
    
    unsigned long time_ =  millis();

    if(time_ - timeStartAllarm  >  TIMEOUT_ALLARM_ms){
        setStatus(STATUS_POWER_OFF);
    }
    
    else if(time_ - timetBlinkAllarm > BLINK_PERIOD_ms){
        blinkAllarmOn = !blinkAllarmOn;

        if(blinkAllarmOn)   LedSocketPwr.setPixelColor(0, LedSocketPwr.Color(COLOR_ALLARM));    
        else    LedSocketPwr.setPixelColor(0, LedSocketPwr.Color(0,0,0));

        LedSocketPwr.show();

        timetBlinkAllarm = time_;
    }

    return 0;
}

// TODO
static inline void readChipTemp(){
    uint8_t temp = 32;
    if(temp != chipTemperature){
        chipTemperature = temp;
        chipTempChange=true;
    }
}

//press time required is: time_press_multipler*PRESS_TIME_ms
static inline bool pressButton(uint8_t time_press_multipler){
    if(digitalRead(BUTTON) == LOW){
        unsigned long time_start = millis();
        //smart delay
        while(millis()-time_start < PRESS_TIME_ms*time_press_multipler){
            if(digitalRead(BUTTON) != LOW) return false;
        }
        return true;
    }
    else return false;
}

static inline void setStatus(uint8_t status){
    switch (status){
        case STATUS_POWER_OFF:
            LedSocketPwr.setPixelColor(0, LedSocketPwr.Color(COLOR_OFF));
            LedSocketPwr.show();
        
            digitalWrite(RELAY_1, RELAY_OFF);
            digitalWrite(RELAY_2, RELAY_OFF);

            status_switch = STATUS_POWER_OFF;
            changes.switchStatus= true;

            if(realPower != 0.0f){
                realPower=0.0f;
                changes.sensors=true;
            }

            currentStatusRoutine=routineStatusOFF;
        break;
        
        case STATUS_POWER_ON:    
            LedSocketPwr.setPixelColor(0, LedSocketPwr.Color(COLOR_ON));
            LedSocketPwr.show();
        
            digitalWrite(RELAY_1, RELAY_ON);
            digitalWrite(RELAY_2, RELAY_ON);

            status_switch = STATUS_POWER_ON;
            changes.switchStatus= true;

            currentStatusRoutine=routineStatusON;
        break;

        case STATUS_POWER_ALLARM:
            LedSocketPwr.setPixelColor(0, LedSocketPwr.Color(COLOR_ALLARM));
            LedSocketPwr.show();
        
            digitalWrite(RELAY_1, RELAY_OFF);
            digitalWrite(RELAY_2, RELAY_OFF);

            status_switch = STATUS_POWER_ALLARM;
            changes.switchStatus= true;

            if(realPower != 0.0f){
                realPower=0.0f;
                changes.sensors=true;
            }

            timeStartAllarm = millis();
            timetBlinkAllarm = timeStartAllarm;
            blinkAllarmOn = true;

            currentStatusRoutine=routineStatusALLARM;
        break;

        default:
        break;
    }
}

static inline void writeFloat(uint8_t* dst, volatile float* toWrite){
    uint32_t* pointer_raw;
    uint32_t raw;
    pointer_raw = (uint32_t*) toWrite;
    raw = (uint32_t)(*pointer_raw);
    dst[3] = (uint8_t) raw;
    dst[2] = (uint8_t) (raw >> 8);
    dst[1] = (uint8_t) (raw >> 16);
    dst[0] = (uint8_t) (raw >> 24);
}
static inline float readFloat(uint8_t* src){
	uint32_t raw32;
	float* pointer_tmpF;

	raw32 = src[3];
    raw32 |= ((uint32_t)src[2]) << 8;
    raw32 |= ((uint32_t)src[1]) << 16;
    raw32 |= ((uint32_t)src[0]) << 24;
    pointer_tmpF = (float*) &raw32;
    return *pointer_tmpF;
}