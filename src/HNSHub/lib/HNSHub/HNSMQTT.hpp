#pragma once

#include "HNSCore.h"

#include <string>

using namespace std;

#define DISABLE_CHIP_TEMPERATURE     true

#define DEFAULT_UPDATE_PERIOD_US  5*1E06 //5 seconds
#define DEFAULT_UPDATE_MIN_PERIOD_US 100*1E03 //100 milliseconds

// useful to build topic
#define PREFIX    std::string("custom")

#define SLASH     std::string("/")
#define COMMAND   std::string("command")
#define ANNOUNCE  std::string("announce");
//status switch 
#define SWITCH_ON       "on"//"ON"
#define SWITCH_OFF      "off"//"OFF"
#define SWITCH_ALLARM   "ALRM"

#define PAYLOAD_ERROR     "ERR"

#define MQTT_DEVICE_TYPE_SIZE       3
#define MQTT_DEVICE_SUBTYPE_SIZE    7

#define MQTT_COMMAND_SIZE     3

typedef enum MQTTDeviceType{
  _NULL_type_=0,
	_sensor_,
	_actuator_,
}MQTTDeviceType;
extern const char* MQTTDeviceTypeString[];

typedef enum MQTTDeviceSubType{
    _NULL_subType_=0,
    _switch_,
    _realPower_, //measure realPower
    _autoSwitch_, // poweroff if power is upper limit (controlled by device)
    _realApparentPower_, //measure real and apparent power
    _realPwrVoltCurrRMS_, //measure realPower voltage RMS and current RMS
    _chipTemperature_
}MQTTDeviceSubType;
extern const std::string MQTTDeviceSubTypeString[];

//extern const char* MQTTDeviceSubTypeString[];

typedef enum MQTTCommand{
  _NULL_command=0,
  _ON_,
  _OFF_
}MQTTCommand;
extern const std::string MQTTCommandString[];

typedef struct MQTTCommand_struct{
	std::string MQTTDeviceID;
	std::string subDevice;
	std::string subID;
	std::string command;

    //used for ack
  std::string command_topic;
}command_struct;

class MQTTMessage{
  public:
      std::string topic;
      std::string payload;

      MQTTMessage(const char* topic, const char* payload){
        this->topic = std::string(topic);
        this->payload = std::string(payload);
      }
      
      MQTTMessage():MQTTMessage("", ""){}

};

class MQTTSubDevice{
  public:
    bool enabled : 4; //specifies is this sub device is used or not
    bool forceUpload : 4; //force upload to without waiting for updatePeriod (but respects updateMinPeriod)

   // MQTTDeviceType type; // sensor or actuator
    MQTTDeviceSubType subType; //use to identify a sub type of device
    uint16_t subID; //identify subType (es subType=switch, subID=3 -> switch_03)

    double lastUpload;  //Time of last upload of MQTTMessage on MQTT Broker
    double updatePeriod_uS; //Period between two normal MQTT publish (routine refresh)
    double updateMinPeriod_uS; //Minimum time between two MQTT publish, flag forceUpload is useless


    MQTTMessage message; //MQTTMessage to publish 

    MQTTSubDevice(const char* topic, const char* payload){
      this->message.topic=topic;
      this->message.payload=payload;

     // this->type=MQTTDeviceType::_NULL_type_;
      this->subType = MQTTDeviceSubType::_NULL_subType_;
      this->enabled = true;
      this->forceUpload = true;
      this->lastUpload=0.0;
      this->updatePeriod_uS=DEFAULT_UPDATE_PERIOD_US;
      this->updateMinPeriod_uS=DEFAULT_UPDATE_MIN_PERIOD_US;
    }
    
    MQTTSubDevice() : MQTTSubDevice("NULL", "NULL"){}

    MQTTSubDevice(std::string MQTTDeviceID, MQTTDeviceSubType subType,  uint16_t sub_id){
      //                    custom/HNSSokcetFF4467FB/power/0
      //                  PREFIX / ID / subID / subTYpe  
      this->message.topic = PREFIX + SLASH + 
                            MQTTDeviceID + SLASH +
                            std::to_string(sub_id)  + SLASH +
                            string(MQTTDeviceSubTypeString[subType]);
      this->subID=sub_id;
      this->subType = subType;
      this->enabled = true;
      this->forceUpload = true;
      this->lastUpload=0.0;
      this->updatePeriod_uS=DEFAULT_UPDATE_PERIOD_US;
      this->updateMinPeriod_uS=DEFAULT_UPDATE_MIN_PERIOD_US;
    }

    // build and return MQTT Announce Message
    // NOTE: it doesn't save inside the message
    std::string getAnnounceMessage(const char* MQTTDeviceID, const char* HNSHub_IP);

    // using internal informtaion update MQTTMessage but doesn't publish!
    virtual res_t updateMQTTMeassge(){ return -1;}
    
    std::string to_string(){
      return string("{topic:") + this->message.topic + string(", payload:") + this->message.payload + string("}"); 
    }
};

//measure temperature of chip
class MQTTSubDevice_chip_temperature : public MQTTSubDevice{
    public:
        float temperature;
      //  MQTTSubDevice_chip_temperature(){}

        MQTTSubDevice_chip_temperature(string MQTTDeviceID,  uint16_t sub_id) 
          : MQTTSubDevice(MQTTDeviceID, MQTTDeviceSubType::_chipTemperature_, sub_id){
            this->temperature=0.0f;
            this->message.payload = std::string("100.00");

            if(DISABLE_CHIP_TEMPERATURE) this->enabled=false;
        }

        virtual res_t updateMQTTMeassge(){
          char buff[16];
          snprintf(buff, 16, "%.2f", this->temperature);
          this->message.payload = buff;

          this->forceUpload = true; //notice changed
          printf("%s\n", this->to_string().c_str());
          //printf("Chip Temperature: %.2f\n", temperature);
        }
};

//publish MQTT message
res_t HNSMQTT_publish(string topic, string payload);

res_t HNSMQTT_init(const char* host, const char* user, const char* password);

res_t HNSMQTT_publishAll();

res_t HNSMQTT_publishDevice(address_t address);

//publish command ACK
// command_topic is original topic received from Broker
// function modify this according and send to Broker
res_t HNSMQTT_confirmCommand(std::string command_topic);

// send announce mqtt message to broker (with payload.type=JSON)
//topic : custom/<device_id>/announce
//payload = device.getMQTTAnnounce; //fisrt must be create with createMQTTAnnounce()
res_t HNSMQTT_announce(address_t address);


//subscribe to command topic of each subDevice
res_t HNSMQTT_subscribeCommand(address_t address);