
#include "HNSMQTT.hpp"

#include <stdio.h>
#include <mosquitto.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include "HNSDatabase.hpp"
#include "HNSAddressMap.hpp"
#include "../HNSCore/HNSError.h"
#include <iostream>
#include "HNSUtil.hpp"

#define STATISTICAL_TEST
#define ECHO_PREFIX "/echo"

const std::string topic_prefix = PREFIX;
const std::string slash_string = SLASH;

const char* MQTTDeviceTypeString[MQTT_DEVICE_TYPE_SIZE]{
  "NULL",
  "sensor",
  "actuator"
};

//const char* MQTTDeviceSubTypeString[MQTT_DEVICE_SUBTYPE_SIZE]{
const std::string MQTTDeviceSubTypeString[MQTT_DEVICE_SUBTYPE_SIZE]{
  "NULL",
  "switch",
  "power",//"realPower",
  "autoSwicth",
  "realApparentPower",
  "realPwrVoltCurrRMS",
  "chipTemperature"
};

const std::string MQTTCommandString[MQTT_COMMAND_SIZE] = {
  "NULL",
  "on",//"ON",
  "off"// "OFF"
};

HNSUtil Utils;
std::string prefix = std::string("/home/living_room/controls/");
struct mosquitto *mosq = NULL;

void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message);
static inline int splitTopic(std::string topic, std::string& device, std::string& command);
static inline int getCommand(std::string& topic, std::string& payload, MQTTCommand_struct& command);

#define DEBUG_MQTT
#define PRINT


res_t HNSMQTT_confirmCommand(std::string command_topic){
  std::string topic = command_topic + std::string("/ACK");
  printf("echo: %s\n", topic.c_str());
  return HNSMQTT_publish(topic, " ");
}

res_t HNSMQTT_publish(string topic, string payload){
  #if defined(PRINT) && defined(DEBUG_MQTT)
    std::string msg = std::string("Topic=") + topic;
    msg += std::string(" Payload= ") + payload;
    Utils.printDebug(__func__, msg.c_str());
  #endif
  
  // TEST WITHOUT MQTT
  
  int res = mosquitto_publish(mosq, NULL, topic.c_str(), payload.length(), payload.c_str(), 0, 0);
  
  if(res != MOSQ_ERR_SUCCESS){
    #if defined(PRINT) && defined(DEBUG_MQTT)
    Utils.printDebug(__func__, "Error in mosquitto_publish()");
    mosquitto_strerror(res);// perror("mosuitto publish");
    #endif
    exit(EXIT_FAILURE);
  }
  
  return 0;
}

res_t HNSMQTT_init(const char* host, const char* user, const char* password){
	int port = 1883;
	int keepalive = 60;
	bool clean_session = true;

  int res = 0;
  res = mosquitto_lib_init();
  if(res != MOSQ_ERR_SUCCESS){
    #if defined(PRINT) && defined(DEBUG_MQTT)
    Utils.printDebug(__func__, "mosquitto_lib_init()");
    mosquitto_strerror(res);//printf("ERRORE mosquitto init\n");
    #endif
  
    exit(EXIT_FAILURE);
  }

  mosq = mosquitto_new(NULL, clean_session, NULL);
  if(mosq == NULL){
    #if defined(PRINT) && defined(DEBUG_MQTT)
    Utils.printDebug(__func__, "mosquitto_new()");
    perror("mosquitto_new");
    #endif
  
    exit(EXIT_FAILURE);
  }

  mosquitto_message_callback_set(mosq, message_callback);


  if(user != NULL && password != NULL){
    mosquitto_username_pw_set(mosq, user, password);
  }
  
  //mosquitto_username_pw_set(mosq, "local_broker", "ucpoc213");
 //printf("try pass \n");

  
  res = mosquitto_connect(mosq, host, port, keepalive);
  if(res != MOSQ_ERR_SUCCESS){
	  #if defined(PRINT) && defined(DEBUG_MQTT)
    Utils.printDebug(__func__, "mosquitto_connect()");
    mosquitto_strerror(res);
    #endif

		exit(EXIT_FAILURE);
	}

printf("try pass \n");

  std::string sub_topic = "custom/HNSSocketPowerAF009A49/0/switch/command"; //"custom/HNSSocketPowerAF0054FF/0/switch/commands"; //prefix + "#";

  //mosquitto_subscribe(mosq, NULL, sub_topic.c_str(), 2);
  res = mosquitto_subscribe(mosq, NULL, sub_topic.c_str(), 0);
  if(res!=MOSQ_ERR_SUCCESS){
  	#if defined(PRINT) && defined(DEBUG_MQTT)
    Utils.printDebug(__func__, "mosquitto_subscribe()");
    mosquitto_strerror(res);
    #endif
  
		exit(EXIT_FAILURE);
  }
  
    sub_topic = "custom/HNSSocketPowerAF019A49/0/switch/command"; //"custom/HNSSocketPowerAF0054FF/0/switch/commands"; //prefix + "#";

  //mosquitto_subscribe(mosq, NULL, sub_topic.c_str(), 2);
  res = mosquitto_subscribe(mosq, NULL, sub_topic.c_str(), 0);
  if(res!=MOSQ_ERR_SUCCESS){
  	#if defined(PRINT) && defined(DEBUG_MQTT)
    Utils.printDebug(__func__, "mosquitto_subscribe()");
    mosquitto_strerror(res);
    #endif
  
		exit(EXIT_FAILURE);
  }

    sub_topic = "custom/HNSSocketPowerAF029A49/0/switch/command"; //"custom/HNSSocketPowerAF0054FF/0/switch/commands"; //prefix + "#";

  //mosquitto_subscribe(mosq, NULL, sub_topic.c_str(), 2);
  res = mosquitto_subscribe(mosq, NULL, sub_topic.c_str(), 0);
  if(res!=MOSQ_ERR_SUCCESS){
  	#if defined(PRINT) && defined(DEBUG_MQTT)
    Utils.printDebug(__func__, "mosquitto_subscribe()");
    mosquitto_strerror(res);
    #endif
  
		exit(EXIT_FAILURE);
  }

  res = mosquitto_loop_start(mosq);
  if(res!=MOSQ_ERR_SUCCESS){
    #if defined(PRINT) && defined(DEBUG_MQTT)
    Utils.printDebug(__func__, "mosquitto_loop_start()");
    mosquitto_strerror(res);
    #endif
    
		exit(EXIT_FAILURE);
  }
  return 0;
}


res_t HNSMQTT_publishDevice(address_t address){

  if(address >= HNSDatabase_deviceList_index){
    error=ERR_INVALID_ADDRESS;
    error_func_happened=__func__;
    return -1;
  }

  HNSDatabase_entry entry = HNSDatabase_deviceList[address];

  if(entry.status==deviceStatus_deleted) {
  	error =ERR_DELETED_ADDRESS;
    error_func_happened=__func__;
    return -1;
  }
  
  //HNSDevice* device = entry.device;
 
  res_t result = 0;
  double time_us = Utils.getTime_us();
  
  MQTTSubDevice** subDevice_array = entry.device->getMQTTsubDeviceArray();
  uint16_t subDeviceArray_size = entry.device->getMQTTsubDeviceArray_size();

  for(uint16_t i=0; i < subDeviceArray_size; i++){
    if( !subDevice_array[i] || !subDevice_array[i]->enabled ) continue;

    if(time_us - subDevice_array[i]->lastUpload < subDevice_array[i]->updateMinPeriod_uS){
      // to early to upload
      continue;
    }

    if(subDevice_array[i]->forceUpload || time_us - subDevice_array[i]->lastUpload >= subDevice_array[i]->updatePeriod_uS){
      // TODO add check result
      result = HNSMQTT_publish(subDevice_array[i]->message.topic, subDevice_array[i]->message.payload);
      time_us = Utils.getTime_us(); // refresh time

      subDevice_array[i]->lastUpload = time_us;
      subDevice_array[i]->forceUpload=false; 
    }
  }

  return 0;
}


void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message){
  
  //#if defined(PRINT) && defined(DEBUG_MQTT)
  std::string msg = std::string("Got MQTT message Payload=");
  msg+=std::string((char*)message->payload);
  msg+=" Topic=" + std::string((char*) message->topic);
  Utils.printDebug(__func__, msg.c_str());
  //#endif

  MQTTCommand_struct command;
  std::string topic = std::string((char*) message->topic);
  std::string payload = std::string((char*) message->payload);

  int res = getCommand(topic, payload, command);

  if(res==0){
    #if defined(PRINT) && defined(DEBUG_MQTT)
    char buf[128];
    snprintf(buf, 128, "MQTTDeviceID=%s\tsubDevice=%s\tsubID=%s\tcommand=%s",
        command.MQTTDeviceID.c_str(), command.subDevice.c_str(), command.subID.c_str(), command.command.c_str());
    Utils.printDebug(__func__, buf);
    #endif
  }
  else{
    #if defined(PRINT) && defined(DEBUG_MQTT)
    Utils.printDebug(__func__, "Error getCommand()");
    #endif
    exit(EXIT_FAILURE);
  }

  address_t address=255;
  res_t res2 = HNSAddressMap_getAddress(command.MQTTDeviceID, &address);
  if(res2==-1){
    #if defined(PRINT) && defined(DEBUG_MQTT)
    Utils.printDebug(__func__, "Error in HNSAddressMap_getAddress()");
    #endif
    return;
  }
  
  #ifdef STATISTICAL_TEST
    std::string echo_topic = std::string(ECHO_PREFIX) + topic;
    HNSMQTT_publish(echo_topic, std::string("Running"));
  #endif
  
  HNSDatabase_mutex.lock();

  HNSDatabase_entry entry = HNSDatabase_deviceList[address];
  res_t result = entry.device->setCommand(command);
  if(result==-1){
    info_error("Errore setCommand()");
  }

  HNSDatabase_mutex.unlock();

  /*
  #ifdef STATISTICAL_TEST
    if(result != -1) HNSMQTT_publish(echo_topic, std::string("Done"));
    else HNSMQTT_publish(echo_topic, std::string("Error"));
  #endif
  */
}





static inline int getCommand(std::string& topic, std::string& payload, MQTTCommand_struct& command){
	
	std::size_t command_string = topic.find(COMMAND.c_str());
	if(command_string == std::string::npos) return -1;
	 
	std::size_t len_prefix = topic_prefix.length() + slash_string.length();
	
	std::size_t pos_slash = topic.find("/", len_prefix);
	if(pos_slash == std::string::npos) return -1;
	
	//printf("len_prefix = %d\tpos_slash=%d\n", len_prefix, pos_slash);
	command.MQTTDeviceID = topic.substr(len_prefix, pos_slash-len_prefix);
	
	std::size_t pos_slash_2 = topic.find("/", pos_slash+1);
	if(pos_slash_2 == std::string::npos) return -1;
	
	command.subID = topic.substr(pos_slash+1, pos_slash_2-pos_slash-1);

	pos_slash = topic.find("/", pos_slash_2+1);
	if(pos_slash == std::string::npos) return -1;

	command.subDevice = topic.substr(pos_slash_2+1, pos_slash-pos_slash_2-1);
	
	command.command = payload;
	
  // for command ACK
  command.command_topic = topic;

  return 0;

}


#define DOUBLE_APICE std::string("\"")
#define COMMA std::string(",")

static inline std::string JSONLine_string(const char* index_name, std::string value){
  std::string line = DOUBLE_APICE + std::string(index_name) + DOUBLE_APICE;
  line += std::string(":");
  line += DOUBLE_APICE + value + DOUBLE_APICE;
  return line;
}

res_t HNSMQTT_announce(address_t address){
  
  HNSDevice* device = HNSDatabase_deviceList[address].device;
  string topic = PREFIX + SLASH + device->getMQTTDeviceID() + SLASH + ANNOUNCE;
  //string payload = device->getMQTTAnnounce();
  std::string payload = "[";

  uint16_t size = device->getMQTTsubDeviceArray_size();
  MQTTSubDevice** subDevice = device->getMQTTsubDeviceArray();

  std::string subDeviceString_array[size];

  uint16_t num_subDevice_to_announce = 0;
  for(uint16_t i=0; i< size; i++){
        

    //if(!subDevice[i] || !subDevice[i]->enabled) continue;
    if(!subDevice[i])continue;
    if(!subDevice[i]->enabled) continue;
    
    subDeviceString_array[num_subDevice_to_announce]
          = subDevice[i]->getAnnounceMessage(device->getMQTTDeviceID().c_str(), device->getHNSHub_IP());
    num_subDevice_to_announce++;
  } 

  for(uint16_t i=0; i< num_subDevice_to_announce; i++){
    payload +=  subDeviceString_array[i];
    if(i != num_subDevice_to_announce-1) payload += COMMA;
  }
  payload += std::string("]");

  res_t result = HNSMQTT_publish(topic, payload);
  
  return result;
}




typedef struct AnnounceMessage_struct{
  std::string sub_type;

  std::string device_id;
  std::string sub_id;
  
  std::string ip_address;
  
  std::string update_topic;
  std::string command_topic;

  std::string topics_prefix;
}AnnounceMessage_struct;

static inline std::string buildJSONString(AnnounceMessage_struct announce);

std::string MQTTSubDevice::getAnnounceMessage(const char* MQTTDeviceID, const char* HNSHub_IP){
  AnnounceMessage_struct announce = {
    //.sub_type = std::string(MQTTDeviceSubTypeString[this->subType]),
    .sub_type = MQTTDeviceSubTypeString[this->subType],
    .device_id = std::string(MQTTDeviceID),
    .sub_id = std::to_string(this->subID),
    .ip_address = std::string(HNSHub_IP)
  };
  announce.topics_prefix=PREFIX;
  announce.update_topic = this->message.topic;//PREFIX + SLASH + announce.device_id + SLASH + announce.sub_id + SLASH + MQTTDeviceSubTypeUpdateString[this->subType];
  announce.command_topic = this->message.topic + SLASH + COMMAND;//PREFIX + SLASH + announce.device_id + SLASH + announce.sub_id + SLASH + COMMAND;

  return buildJSONString(announce);
}


static inline std::string buildJSONString(AnnounceMessage_struct announce){
  std::string result = std::string("{"); 

  result += JSONLine_string("device_id", announce.device_id) +COMMA;
  result += JSONLine_string("sub_id", announce.sub_id) +COMMA;
  result += JSONLine_string("ip_address", announce.ip_address) +COMMA;
  result += JSONLine_string("sub_type", announce.sub_type) + COMMA;
  result += JSONLine_string("update_topic", announce.update_topic) + COMMA;
  result += JSONLine_string("command_topic", announce.command_topic) + COMMA;

  result += JSONLine_string("topics_prefix", announce.topics_prefix);

  result += std::string("}"); 

  return result;
}

res_t HNSMQTT_subscribeCommand(address_t address){

}

/*

//https://gist.github.com/piccaso/f463f6ad134a8a2479e62e6e2349e7c0#file-lobmosq-c-L28
//  needs libmosquitto-dev
//  $ gcc -o libmosq libmosq.c -lmosquitto

#include <stdio.h>
#include <mosquitto.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void mosq_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str)
{
	// Pring all log messages regardless of level.

  switch(level){
    //case MOSQ_LOG_DEBUG:
    //case MOSQ_LOG_INFO:
    //case MOSQ_LOG_NOTICE:
    case MOSQ_LOG_WARNING:
    case MOSQ_LOG_ERR: {
      printf("%i:%s\n", level, str);
    }
  }
}

struct mosquitto *mosq = NULL;
char *topic = NULL;
void mqtt_setup(){

	char *host = "localhost";
	int port = 1883;
	int keepalive = 60;
	bool clean_session = true;
  topic = "/testtopic";

  mosquitto_lib_init();
  mosq = mosquitto_new(NULL, clean_session, NULL);
  if(!mosq){
		fprintf(stderr, "Error: Out of memory.\n");
		exit(1);
	}

  mosquitto_log_callback_set(mosq, mosq_log_callback);

  if(mosquitto_connect(mosq, host, port, keepalive)){
		fprintf(stderr, "Unable to connect.\n");
		exit(1);
	}
  int loop = mosquitto_loop_start(mosq);
  if(loop != MOSQ_ERR_SUCCESS){
    fprintf(stderr, "Unable to start loop: %i\n", loop);
    exit(1);
  }
}

int mqtt_send(char *msg){
  return mosquitto_publish(mosq, NULL, topic, strlen(msg), msg, 0, 0);
}

int main(int argc, char *argv[])
{
  mqtt_setup();
  int i = -1000;
  char *buf = malloc(64);
  while(1){
    sprintf(buf,"i=%i",i++);
    int snd = mqtt_send(buf);
    if(snd != 0) printf("mqtt_send error=%i\n", snd);
    usleep(100000);
  }
}
*/
