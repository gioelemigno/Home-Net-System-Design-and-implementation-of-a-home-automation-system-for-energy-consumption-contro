#include "HNSSocketPower.h"
#include "HNSDevice_class.hpp"
#include "HNSCore.h"
#include "HNSMQTT.hpp"
#include "HNSSocketV2.h"
#include <stdint.h>
#include <string>
#include <exception>

#define INDEX_SUBDEVICE_CHIP_TEMP   0
#define INDEX_SUBDEVICE_SUBSOCKET_ARRAY 1
#define NUMBER_COMPONET_SUBSOCKET   3

#define MIN_PERIOD_REFRESH_uS   150E03 // 150E03//250E03

#define CHECK_BITS(bit_word, bit_to_check) (((bit_word) & (bit_to_check))==(bit_to_check))


static inline void writeFloat(uint8_t* dst, float* toWrite){
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

static inline socketTypeSensor_t getSocketTypeSensor(uint8_t socketSetting);


HNSSocketPower::HNSSocketPower(address_t address, uint32_t HNSCodeID, const char* HNSHub_MAC, const char* HNSHub_IP)
    : HNSDevice(address, (uint16_t) HNSModel_HNSSocketPower, HNSCodeID, HNSHub_MAC, HNSHub_IP){
    
    this->n_subSocket = 0;
    this->subSocketType=NULL;
    this->socketSetting=NULL;
}


// TODO get information from device using HNSNet 
res_t HNSSocketPower::init(){ 

    // TODO get information from device using HNSNet 
    //and set this->n_subSocket and this->socketSetting
        this->n_subSocket = 1;
        this->socketSetting = (uint8_t*) malloc(sizeof(uint8_t) * this->n_subSocket);
        assert(this->socketSetting != NULL && "Failed malloc(), dude, you have a serious memory problem!");
        this->socketSetting[0] = REAL_POWER | SWITCH | AUTO_SWITCH_PWR_LIMIT;
    // end use of HNSNet


    /*   ____________________ _______ ________________ _______ ________ _ _
        |___0______|___1_____ ___2___ ___3____|___4__ ___5____ ___6____|_ _
        |CHIP_TEMP |        SUB SOCKET_0      |       SUB_SOCKET_1     |     
    */
    
    this->subSocketType = (socketTypeSensor_t*) malloc(sizeof(socketTypeSensor_t) * this->n_subSocket);
    assert(this->subSocketType != NULL && "Failed malloc(), dude, you have a serious memory problem!");
    for(uint8_t i=0; i<this->n_subSocket; i++){
        this->subSocketType[i] = getSocketTypeSensor(socketSetting[i]);
    }
        
    // 1 is for temperature of chip
    // 1 + this->n_subSocket*sizeof(MQTTSubDevice)
    //this->MQTTsubDeviceArray_size = 1 + (this->n_subSocket*sizeof(MQTTSubDevice)) *NUMBER_COMPONET_SUBSOCKET;
            this->MQTTsubDeviceArray_size = 1 + (this->n_subSocket*NUMBER_COMPONET_SUBSOCKET);
    this->MQTTsubDeviceArray = (MQTTSubDevice**) malloc(sizeof(MQTTSubDevice*)*this->MQTTsubDeviceArray_size);

    // with (std::nothrow), it doesn't throw excpetion and on error return null as malloc()
    //this->MQTTsubDeviceArray = new (std::nothrow) MQTTSubDevice[MQTTsubDeviceArray_size];
    //assert(this->MQTTsubDeviceArray != NULL && "Failed new(), dude, you have a serious memory problem!");

    this->MQTTsubDeviceArray[INDEX_SUBDEVICE_CHIP_TEMP] = new (std::nothrow) MQTTSubDevice_chip_temperature(this->MQTTDeviceID, 0);
    assert(this->MQTTsubDeviceArray[INDEX_SUBDEVICE_CHIP_TEMP] != NULL && "Failed new(), dude, you have a serious memory problem!");

    uint8_t end = INDEX_SUBDEVICE_SUBSOCKET_ARRAY + (NUMBER_COMPONET_SUBSOCKET * this->n_subSocket);
    uint8_t i = INDEX_SUBDEVICE_SUBSOCKET_ARRAY;
    uint8_t subSocket = 0;
    while(i < end){
        if(CHECK_BITS(this->socketSetting[subSocket], SWITCH)){
            this->MQTTsubDeviceArray[i]=new (std::nothrow) MQTTSubDevice_socket_swicth(this->MQTTDeviceID, subSocket);
            assert(this->MQTTsubDeviceArray[i]!= NULL && "Failed new(), dude, you have a serious memory problem!");
        }
        else this->MQTTsubDeviceArray[i]=NULL;
        i++;

        if(CHECK_BITS(this->socketSetting[subSocket], SWITCH | REAL_POWER | AUTO_SWITCH_PWR_LIMIT)){
            this->MQTTsubDeviceArray[i]=new (std::nothrow) MQTTSubDevice_socket_autoSwitch(this->MQTTDeviceID, subSocket);
            assert(this->MQTTsubDeviceArray[i]!= NULL && "Failed new(), dude, you have a serious memory problem!");
        }
        else this->MQTTsubDeviceArray[i]=NULL;
        i++;

        switch ( subSocketType[subSocket] ){
            case socketTypeSensor_t::realPower_sensor:
                this->MQTTsubDeviceArray[i]=new (std::nothrow) MQTTSubDevice_socket_realPower(this->MQTTDeviceID, subSocket);
                assert(this->MQTTsubDeviceArray[i]!= NULL && "Failed new(), dude, you have a serious memory problem!");
            break;
            
            default:
                this->MQTTsubDeviceArray[i]=NULL;
                printf("NOT SUPPORTED THIS TYPE OF SOCKET SENSOR YET : unknown type %d\n", subSocketType[subSocket]);
                exit(-1);
            break;
            }

        i++;
    }
    
    this->sizeDataRefreshAll = sizeof(uint8_t); //temperature of chip
    for(uint8_t i=0; i< this->n_subSocket; i++){
        if(CHECK_BITS(this->socketSetting[i], SWITCH)) this->sizeDataRefreshAll += sizeof(uint8_t); //status switch
        if(CHECK_BITS(this->socketSetting[i], SWITCH | REAL_POWER | AUTO_SWITCH_PWR_LIMIT)) this->sizeDataRefreshAll += sizeof(float);//power_limit
        uint8_t n_float = 0;
        switch (this->subSocketType[i]){
            case socketTypeSensor_t::realPower_sensor:
                n_float=1;
            break;
            
            default:
                n_float=0;
                printf("NOT SUPPORTED THIS TYPE OF SOCKET SENSOR YET");
                exit(-1);
            break;
        }
        this->sizeDataRefreshAll += (n_float * sizeof(float));

    }
    
    this->initialized = true;

    // ONLY FOR TES, REMOVE
   // this->command.command = MQTTCommand::_ON_;
    //this->command.subID = 0;
    //this->command.subDevice = MQTTDeviceSubType::_switch_;
    //this->command.executed=false;
}

MQTTsubSocket_t* HNSSocketPower::getSubSocket(uint16_t _subID){
    /*   ____________________ _______ ________________ _______ ________ _ _
        |___0______|___1_____ ___2___ ___3____|___4__ ___5____ ___6____|_ _
        |CHIP_TEMP |        SUB SOCKET_0      |       SUB_SOCKET_1     |     
    */
    uint16_t index = 1 + _subID*3; // 1 beacuse first element is chip temperature
    if(index >= this->MQTTsubDeviceArray_size){
        printf("invalid subID\n");
        return NULL;
    }
    
    return (MQTTsubSocket_t*) &(this->MQTTsubDeviceArray[index]);
}


res_t HNSSocketPower::refresh(){
    double time_us = _getTime_us(); 
    if(time_us - this->lastRefresh_uS < MIN_PERIOD_REFRESH_uS){
        return 0;
    }
    
    res_t res;
    if(this->command.executed ==  false){
        uint16_t status_to_set;
        switch (this->command.command){
            case MQTTCommand::_ON_:
                status_to_set=STATUS_POWER_ON;
            break;
            
            case MQTTCommand::_OFF_:
                status_to_set=STATUS_POWER_OFF;
            break;
            
            default:
                printf("unkonwn command\n");
                return -1;
            break;
        }
        res = this->setSwicthStatus(this->command.subID, status_to_set);
        this->lastRefresh_uS=_getTime_us();

        if(res == -1){
            printf("Errore set command\n");
            return -1;
        }

        // command ACK
        HNSMQTT_confirmCommand(this->command.command_topic);
        
        this->command.executed=true;
    }
    else{
        res = this->refreshAll();
        this->lastRefresh_uS=_getTime_us();
        //if(res != -1) printf("success!\n");
        return res; 
    }
    return 0;
}       
        
std::string HNSSocketPower::toString(){} 


/*
* MASTER:  size 0 | type REFRSH
*
* SLAVE:   size 1 
*          type RSP_ONLY_TEMP
*          data chipTemperature0
*
* SLAVE:   size 0
*          type RSP_NO_CHANGE
*
* SLAVE:   size x
*          type RSP_REFRESH_n
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
*/
res_t HNSSocketPower::refreshOnlyChange(){
    packet_t packet;
    packet.recipient = this->address;
    packet.size=0;
    packet.type=REFRESH;

    res_t res = HNSRouting_write(&packet);
    if(res==-1)  return -1;
  
    res = HNSRouting_read(&packet);
    if(res==-1)return -1;
    
    switch (res){
        case 0:
            if(packet.type == RSP_NO_CHANGE)    return 0;
            else {
                error = ERR_TYPE;
                error_func_happened = __func__;
                return -1;
            }   
        break;
    
        case 1:
            if(packet.type == RSP_ONLY_TEMP){
                MQTTSubDevice_chip_temperature* chipTemp = (MQTTSubDevice_chip_temperature*) (this->MQTTsubDeviceArray[INDEX_SUBDEVICE_CHIP_TEMP]) ;
                float chip_temp = (float) packet.data[0];

                // REMOVE CHECK BECAUSE DEVICES TELL US IS CHANGED!
                //if(chipTemp->temperature != chip_temp){
                    chipTemp->temperature = chip_temp;
                    chipTemp->updateMQTTMeassge();
                //}
                return 0;
            }
            else{
                error = ERR_TYPE;
                error_func_happened = __func__;
                return -1;
            }
        break;

        default:

            // TODO ..... !!!
        break;
    }

    return -1;
}

res_t HNSSocketPower::refreshAll(){
    packet_t packet;
    packet.recipient = this->address;
    packet.size=0;
    packet.type=REFRESH_ALL;

    res_t res = HNSRouting_write(&packet);
    if(res==-1)  return -1;
  
    res = HNSRouting_read(&packet);
    if(res==-1) {
        return -1;
    }
    if(res== this->sizeDataRefreshAll /*&& packet.type == RSP_REFRESH_ALL*/){
        uint8_t index_data = sizeof(uint8_t);//temperature
        MQTTSubDevice_chip_temperature* chipTemp = 
            (MQTTSubDevice_chip_temperature*) (this->MQTTsubDeviceArray[INDEX_SUBDEVICE_CHIP_TEMP]) ;
        float chip_temp = (float) packet.data[0];

        if(chipTemp->temperature != chip_temp){
            chipTemp->temperature = chip_temp;
            chipTemp->updateMQTTMeassge();
        }

        MQTTsubSocket_t* subSocket = (MQTTsubSocket_t*)( &(this->MQTTsubDeviceArray[INDEX_SUBDEVICE_CHIP_TEMP+1]) );
        for(uint8_t i=0; i<this->n_subSocket; i++){
            if(CHECK_BITS(this->socketSetting[i], SWITCH) ){
                if(subSocket[i].socket_switch == NULL){
                    printf("TO DISABLE A TOPIC JUST PUT ITS ENABLE FLAG TO FALSE");
                    exit(-1);
                }
                
                if(subSocket[i].socket_switch->status != packet.data[index_data]){
                    subSocket[i].socket_switch->status = packet.data[index_data];
                    subSocket[i].socket_switch->forceUpload=true;
                    // TODO add read result
                    subSocket[i].socket_switch->updateMQTTMeassge();
                }
                index_data += sizeof(uint8_t);
            }

            if(CHECK_BITS(this->socketSetting[i], SWITCH | AUTO_SWITCH_PWR_LIMIT | REAL_POWER)){
                if(subSocket[i].socket_autoSwitch == NULL){
                    printf("TO DISABLE A TOPIC JUST PUT ITS ENABLE FLAG TO FALSE");
                    exit(-1);
                }
                float pwr_limit = readFloat(packet.data+index_data);
                if(subSocket[i].socket_autoSwitch->powerLimit !=  pwr_limit){
                    subSocket[i].socket_autoSwitch->powerLimit = pwr_limit;
                    subSocket[i].socket_autoSwitch->forceUpload = true;

                    // TODO add read result
                    subSocket[i].socket_autoSwitch->updateMQTTMeassge();
                }

                index_data += sizeof(float);
            }

            if(subSocket[i].socket_sensor){ //if == null => subSocket doesn't have sensor  
                float real_pwr;
                MQTTSubDevice_socket_realPower* MQTTSensor;
                switch (this->subSocketType[i]){
                    case socketTypeSensor_t::realPower_sensor:
                        MQTTSensor = (MQTTSubDevice_socket_realPower*) subSocket[i].socket_sensor;
                        real_pwr = readFloat(packet.data+index_data);
                        if(MQTTSensor->realPower != real_pwr){
                            MQTTSensor->realPower = real_pwr;
                            MQTTSensor->forceUpload=true;

                            // TODO add read result
                            MQTTSensor->updateMQTTMeassge();

                        }
                        index_data += sizeof(float);
                    break;
                    
                    default:
                        printf("NOT SUPPORTED THIS TYPE OF SOCKET SENSOR YET");
                        exit(-1);
                    break;
                }
            }
        }
        return 0;
    }
    else{
        printf("excpeted = %d, received = %d\n", this->sizeDataRefreshAll, packet.size);
        error=ERR_UNEXPECTED_DATA_SIZE;
        error_func_happened=__func__;
        return -1;
    }
}


res_t HNSSocketPower::setCommand(MQTTCommand_struct& _command){
    // TODO use a Map to convert command and subDevice
    this->command.executed=true;
    if(_command.command.compare(MQTTCommandString[MQTTCommand::_ON_]) == 0){
        this->command.command=MQTTCommand::_ON_;
    }
    else if(_command.command.compare(MQTTCommandString[MQTTCommand::_OFF_]) == 0){
        this->command.command=MQTTCommand::_OFF_;
    }
    else {
        printf("ERRORE UNKNOWN MQTT COMMAND\n");
        return -1;
    }

	try{
		this->command.subID = (uint16_t) std::stoi(_command.subID);
	}
	catch (std::exception& e){
        printf("Error in stoi()");
		return -1;
	}

    if(_command.subDevice.compare(MQTTDeviceSubTypeString[MQTTDeviceSubType::_realPower_])==0){
        this->command.subDevice = MQTTDeviceSubType::_realPower_;
        printf("NO COMMAND FOR THIS SUB DEVICE YET!\n");
        return -1;
    }
    else if(_command.subDevice.compare(MQTTDeviceSubTypeString[MQTTDeviceSubType::_switch_])==0){
        this->command.subDevice = MQTTDeviceSubType::_switch_;
    }
    else{
        printf("ERRORE UNKNOWN MQTT subDevice\n");
        return -1;
    }

    this->command.command_topic = _command.command_topic;
    this->command.executed=false; //must to be execute
    return 0;
}



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
*************************************************************************/
res_t HNSSocketPower::setSwicthStatus(uint16_t subID, uint16_t status_to_set){


    printf("run set switch\n");
    uint8_t wordChanges = 0; // clear struct
    subSocketChanges_t* what_to_set = (subSocketChanges_t*) &wordChanges;

    // indicates subSocket and what set to new status (status_to_set)
    what_to_set->switchStatus = true;
    what_to_set->indexSubSocket = subID;
    
    packet_t packet;
    packet.recipient = this->address;
    packet.size=1;
    packet.type=(status_to_set==STATUS_POWER_ON)? SET_STATUS_ON : SET_STATUS_OFF;
    packet.data[0]=wordChanges;

    res_t res = HNSRouting_write(&packet);
    if(res==-1)  return -1;
        printf("run set switch OK READ\n");

    res = HNSRouting_read(&packet);
    if(res==-1) return -1;
        printf("run set switch OK WRITE\n");

    uint8_t expected_type = (status_to_set==STATUS_POWER_ON)? RSP_SET_STATUS_ON:RSP_SET_STATUS_OFF;
    switch(res){
        case 1:
        
            if(packet.type != expected_type || packet.data[0]!=wordChanges){
                printf("errore setting \n\tpacket.type=%d, expected=%d\n\tpacket0=%d, expeted=%d\n",
                        packet.type, expected_type, packet.data[0], wordChanges);
                return -1;
            }
            
        break;

        default:
            error=ERR_UNEXPECTED_DATA_SIZE;
            error_func_happened=__func__;
            return -1;
        break;
    }
    
    return 0;
}

static inline socketTypeSensor_t getSocketTypeSensor(uint8_t socketSetting){
    if(CHECK_BITS(socketSetting, REAL_POWER | APPARENT_POWER)) return socketTypeSensor_t::realApparentPower_sensor;
    if(CHECK_BITS(socketSetting, REAL_POWER | I_RMS | V_RMS)) return socketTypeSensor_t::realPower_volt_curr_rms_sensor;
    if(CHECK_BITS(socketSetting, REAL_POWER)) return socketTypeSensor_t::realPower_sensor;
    
    return socketTypeSensor_t::undefined_sensor;
}