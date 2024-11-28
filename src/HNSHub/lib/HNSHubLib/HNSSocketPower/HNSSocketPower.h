#pragma once

#include "HNSDevice_class.hpp"
#include "HNSCore.h"
#include "HNSSocketV2.h"

#include <stdint.h>
#include <stdbool.h>


#define DISABLE_AUTO_SWICTH          true

typedef enum socketTypeSensor_t{
    undefined_sensor=0,
    realPower_sensor = 1,
    realApparentPower_sensor,
    realPower_volt_curr_rms_sensor
}socketTypeSensor_t;


//simple switch
class MQTTSubDevice_socket_swicth : public MQTTSubDevice{
    public:
        uint8_t status; // STATUS_POWER_ON or STATUS_POWER_OFF
        
        MQTTSubDevice_socket_swicth(std::string MQTTDeviceID,  uint16_t sub_id)
            :MQTTSubDevice(MQTTDeviceID, MQTTDeviceSubType::_switch_, sub_id){
            //this->type = MQTTDeviceType::_actuator_;
            this->subType = MQTTDeviceSubType::_switch_;
            this->status=STATUS_POWER_OFF;
            
            this->message.payload = std::string(SWITCH_OFF);
        }

        virtual res_t updateMQTTMeassge(){
            switch (this->status){
                case STATUS_POWER_ON:
                    this->message.payload = SWITCH_ON;
                break;
                
                case STATUS_POWER_OFF:
                    this->message.payload = SWITCH_OFF;
                break;

                case STATUS_POWER_ALLARM:
                    this->message.payload = SWITCH_ALLARM;
                break;

                default:
                    this->message.payload = PAYLOAD_ERROR;
                break;
            }
            printf("%s\n", this->to_string().c_str());
            this->forceUpload = true; //notice changed
                /*
                char* name_status;
                switch (status){
                    case STATUS_POWER_ON:
                        name_status = "POWER_ON";
                    break;
                
                    case STATUS_POWER_OFF:
                        name_status ="POWER_OFF";
                    break;

                    case STATUS_POWER_ALLARM:
                        name_status = "POWER_ALLARM";
                    break;

                    default:
                        name_status = "UNKNOWN !!!";
                    break;
                }
                printf("Status socket = %s\n", name_status); 
                */
            return 0;
        }
};
 
//poweroff if realPower is upper limit (controlled by device)
class MQTTSubDevice_socket_autoSwitch : public MQTTSubDevice{
    public:
       // uint8_t status; // STATUS_POWER_ON or STATUS_POWER_OFF
        double powerLimit; //indicates powerLimit setted in Device 
        
        MQTTSubDevice_socket_autoSwitch(std::string MQTTDeviceID,  uint16_t sub_id)
            :MQTTSubDevice(MQTTDeviceID, MQTTDeviceSubType::_autoSwitch_, sub_id){
            
            this->powerLimit=DEFAULT_POWER_LIMIT;

            //inizialize string to use correct lenght for char*
            this->message.payload=std::string("2000"); //2KW
            if(DISABLE_AUTO_SWICTH) this->enabled=false;
        }

        virtual res_t updateMQTTMeassge(){
            char buff[16];
            snprintf(buff, 16, "%.2f", this->powerLimit);
            this->message.payload = buff;

            this->forceUpload = true; //notice changed
            //printf("Power limit = %.2f\n", this->powerLimit);
            printf("%s\n", this->to_string().c_str());
            return 0;
        }

};

//measure only realPower
class MQTTSubDevice_socket_realPower : public MQTTSubDevice{
    public:
        double realPower;

        MQTTSubDevice_socket_realPower(std::string MQTTDeviceID,  uint16_t sub_id)
            :MQTTSubDevice(MQTTDeviceID, MQTTDeviceSubType::_realPower_, sub_id){
            
            // this->type = MQTTDeviceType::_sensor_;
            //this->subType = MQTTDeviceSubType::_realPower_;

            this->message.payload=std::string("0.0");
            this->realPower = 0.0f;
        }

        virtual res_t updateMQTTMeassge(){
            char buff[16];
            snprintf(buff, 16, "%.2f", this->realPower);
            this->message.payload = buff;

            this->forceUpload = true; //notice changed

            printf("%s\n", this->to_string().c_str());
            //printf("Real power = %.2f\n", this->realPower);
            return 0;
        }
};

//measure realPower and apparentPower
class MQTTSubDevice_socket_realApparentPower : public MQTTSubDevice{
    public:
        double realPower;
        double apparentPower;

        MQTTSubDevice_socket_realApparentPower() {
            //this->type = MQTTDeviceType::_sensor_;
            this->subType = MQTTDeviceSubType::_realApparentPower_;

            this->realPower=0.0f;
            this->apparentPower=0.0f;
        }
};

//measure realPower, voltage RMS and current RMS
class MQTTSubDevice_socket_realPowerVolategCurrentRMS : public MQTTSubDevice{
    public:
        double realPower;
        double voltageRMS;
        double currentRMS;

        MQTTSubDevice_socket_realPowerVolategCurrentRMS() {
            //this->type = MQTTDeviceType::_sensor_;
            this->subType = MQTTDeviceSubType::_realPwrVoltCurrRMS_;
            
            this->realPower=0.0f;
            this->voltageRMS=0.0f;
            this->currentRMS=0.0f;

        }
};

//this struct is used as mask to easy access to subDevices of socket
typedef struct MQTTsubSocket_t{
    MQTTSubDevice_socket_swicth* socket_switch; //switch (ON / OFF)

    // smart switch, it can be auto switch off if power is upper of limit
    MQTTSubDevice_socket_autoSwitch* socket_autoSwitch; 
    
    //sensor of socket, it can measure:
    //  > only real_power -> subType = MQTTDeviceSubType::_realPower_
    //  > real_power + apparent_power -> subType = MQTTDeviceSubType::_realApparentPower_
    //  > real_power + I_rms + V_rms -> subType = MQTTDeviceSubType::_voltCurrRMS_
    MQTTSubDevice* socket_sensor;
}MQTTsubSocket_t;

class HNSSocketPower : public HNSDevice{
    
    public:
        HNSSocketPower(address_t address, uint32_t HNSCodeID, const char* HNSHub_MAC, const char* HNSHub_IP);

        HNSSocketPower(address_t address) 
                    : HNSSocketPower(address, 0xFF23, "54FF", "0.0.0.0"){}
        
        HNSSocketPower(address_t address, uint16_t uid) 
                    : HNSSocketPower(address, uid, "54FF", "0.0.0.0"){}
        
        //initialize new device, Hub requests information of setup to device (use RS485 HNSNet)
        // (example: HNSocketPower requests to device the number of subSocket and their configuration)
        virtual res_t init();

        //refresh data from device (use RS485 HNSNet)
        virtual res_t refresh();       
        
        // TODO return human readable string describes device
        virtual std::string toString(); 
        
        //convert MQTTCommand_struct to MQTTCommandDevice and save it.
        //Command will be execute in refresh()
        virtual res_t setCommand(MQTTCommand_struct& _command);
        
        //set status of swicth_subID
        // status_to_set can be: STATUS_POWER_OFF or STATUS_POWER_ON 
        res_t setSwicthStatus(uint16_t subID, uint16_t status_to_set);
    private:

        // return subSocket_subID. If error occured, return NULL
        MQTTsubSocket_t* getSubSocket(uint16_t subID);

        uint8_t n_subSocket; //indicates number of subSocket and type of each
        uint8_t* socketSetting; //indicate features of device
        socketTypeSensor_t* subSocketType; // indicate type of sensor assigned in init()

        uint8_t sizeDataRefreshAll; //size of data packet as response of REFRESH_ALL
        
        res_t refreshOnlyChange(); //ask to device only changes
        res_t refreshAll();// ask to device all quantities
};



