#pragma once

#include "HNSCore.h"
#include "HNSMQTT.hpp"

#include <assert.h>
#include <stdint.h>
#include <string>
#include <mutex>          // std::mutex, std::unique_lock
#include <stdbool.h>
#include <stdio.h>


static inline double _getTime_us(){
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return  ts.tv_sec*1E06+ts.tv_nsec*1E-03;
}


typedef struct MQTTCommandDevice {
  bool executed;  //true: nothing to do, false: nedd to be runned
  MQTTCommand command;  //integer that indicates command
  MQTTDeviceSubType subDevice; //sub device that executes command
  uint16_t subID; //subID of sub device that executes command  
  //used for ack
  std::string command_topic;
}MQTTCommandDevice;

class HNSDevice{

  protected:
    uint16_t model; //Indicates HNSModel (type of device)
    address_t address; //address in RS485 HNSNet
    uint32_t HNSCodeID; //unique code allows to recognize a specific device
    std::string deviceNameHumFriendly; //name of device uses for human interaction (TODO) 
    double lastRefresh_uS; //time last refresh (HNSNet)


    std::string MQTTDeviceID; //Name identify a device to MQTT Broker
    MQTTMessage MQTTAnnounce; //MQTT message to upload when device connectes to HNSNet
    
    MQTTSubDevice** MQTTsubDeviceArray; //contains MQTTSubDevice pointers
    uint16_t MQTTsubDeviceArray_size;

    const char* HNSHub_MAC;
    const char* HNSHub_IP;

    bool initialized; // you must perform init() to inizialize device
   
    MQTTCommandDevice command; //MQTT command, it will execute in refresh()

  public:
    HNSDevice(address_t address, uint16_t model, uint32_t HNSCodeID, const char* HNSHub_MAC, const char* HNSHub_IP){
      this->address =address;
      this->model = model;
      this->deviceNameHumFriendly="no_name_device";
      this->HNSCodeID = HNSCodeID;

      this->HNSHub_IP = HNSHub_IP;
      this->HNSHub_MAC = HNSHub_MAC;

      this->lastRefresh_uS = 0.0f;

      //format MQTT_DeviceID:
      //  	MAC HNSHub: 00 E3 E4 BF FF 12
	    //    HNSCode: F3 F9 23 02
      // => <HNSModel><least 4 digits HNSCode><least 4 digits MAC HNSHub>
      //  HNSSocket2302FF12
      char code[16]; 
	    snprintf(code, 16, "%04X", (uint16_t)HNSCodeID);
      
      std::string str_MAC = this->HNSHub_MAC; 
      std::string MAC_4 = str_MAC.substr(str_MAC.length()-4, 4);

      const char* model_name = HNSModel_getModelName(model);
      assert(model_name != NULL && "Invalid Model! You must check valid of model before call HNSDevice's costructor!");
      
      this->MQTTDeviceID = model_name + std::string(code) + MAC_4;
    
      MQTTAnnounce.topic = "NULL";
      MQTTAnnounce.payload ="NULL";

      MQTTsubDeviceArray = NULL;
      MQTTsubDeviceArray_size = 0;
      
      this->initialized=false;
      this->command.executed = true;
    }

    //initialize new device, Hub requests information of setup to device
    // (example: HNSocketPower requests to device the number of subSocket and their configuration)
    virtual res_t init();

    //refresh data from device (use RS485 HNSNet)
    virtual res_t refresh();       
    
    // TODO return human readable string describes device
    virtual std::string toString(); 
    
    //convert MQTTCommand_struct to MQTTCommandDevice and save it.
    //Command will be execute in refresh()
    virtual res_t setCommand(MQTTCommand_struct& command);

    //read MQTTsubDeviceArray and build MQTTAnnounce according to
    virtual res_t createMQTTAnnounce(std::string HNSHub_IPAddress, 
                                        std::string HNSHub_MACAddress){return -1;}


    address_t getAddress(){return address;}
    uint16_t getModel(){return model;}
    uint32_t getHNSCodeID(){return this->HNSCodeID;}
    std::string getDeviceName(){return this->deviceNameHumFriendly;}
    std::string getMQTTDeviceID(){return this->MQTTDeviceID;}
    MQTTMessage getMQTTAnnounceMessage(){return this->MQTTAnnounce;}
    uint16_t getMQTTsubDeviceArray_size(){return this->MQTTsubDeviceArray_size;}
    MQTTSubDevice** getMQTTsubDeviceArray(){return this->MQTTsubDeviceArray;}
    const char* getHNSHub_IP(){return this->HNSHub_IP;}
    const char* getHNSHub_MAC(){return this->HNSHub_MAC;}


    ~HNSDevice(){
      if(this->initialized){
        for(uint16_t i=0; i<this->MQTTsubDeviceArray_size; i++){
          if(this->MQTTsubDeviceArray[i]) delete this->MQTTsubDeviceArray[i];
        }
      }
    }
};


