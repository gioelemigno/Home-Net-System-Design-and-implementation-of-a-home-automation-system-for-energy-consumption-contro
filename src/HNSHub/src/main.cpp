
#include "HNSCore.h"
#include "HNSSync.hpp"
#include "HNSDevice_class.hpp"
#include "HNSDatabase.hpp"
#include "HNSRefresh.hpp"
#include "sigint_handler.h"
#include "HNSMQTT.hpp"
#include "HNSHubInfo.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <mutex>

static inline void refresh_pusblish();

//#define IP "151.25.91.50"
#define IP "192.168.12.187"

//#define INTERFACE   "wlan0" //RaspberryPi Zero W
#define INTERFACE   "wlo1"  //ASUS

int main(int argc, char** argv){

	const char* wlan_interface = INTERFACE;
	const char* Broker_IP = IP;
	const char* MQTT_user = NULL;
	const char* MQTT_pass =  NULL;

	switch(argc){
		case 1:
			printf("Using default setting...\n");
			printf("Usage ./main <wlan interface> <Broker IP> <MQTT user> <MQTT password>");
		break;
		
		case 2:
			wlan_interface = argv[1];
			printf("set wlan_interface = <%s>\n", wlan_interface);
		break;

		case 3:
			wlan_interface = argv[1];
			printf("set wlan_interface = <%s>\n", wlan_interface);

			Broker_IP = argv[2];
			printf("set Broker_IP = <%s>\n", Broker_IP);
		break;

		case 5:
			wlan_interface = argv[1];
			printf("set wlan_interface = <%s>\n", wlan_interface);

			Broker_IP = argv[2];
			printf("set Broker_IP = <%s>\n", Broker_IP);

			MQTT_user = argv[3];
			printf("set MQTT_user = <%s>\n", MQTT_user);

			MQTT_pass = argv[4];
			printf("set MQTT_pass = <%s>\n", MQTT_pass);

		break;

		default:
			printf("Usage ./main <wlan interface> <Broker IP> <MQTT user> <MQTT password>");
			return -1;
		break;
	}
	int res = HNSHubInfo.init(INTERFACE);
	if(res!= -1) printf("%s\n", HNSHubInfo.to_string().c_str());

	sigint_handler_init();
	HNSRouting_init(HNSRouting_role_HNSHub);
	//HNSError_DEBUG();
	HNSDatabase_init(HNSHubInfo.getIPAddress().c_str(), HNSHubInfo.getMACAddress().c_str());
	
	HNSMQTT_init(Broker_IP, MQTT_user, MQTT_pass);

	for(int k = 0; k > -1 ; k++){
		if(k == 0 || k ==1 || k == 2){
			HNSDatabase_mutex.lock();
			printf("sync\n");
			address_t new_device;
			if(HNSSync_sync(&new_device)==0) {
				printf("================\033[1;32m NEW DEVICE \033[0m================\n");
				//HNSDatabase_deviceList[new_device].device->createMQTTAnnounce(HNSHubInfo.getIPAddress(), HNSHubInfo.getMACAddress());
				HNSMQTT_announce(new_device);
				//printf(HNSDatabase_deviceList[new_device].device->getMQTTAnnounce().c_str());
			}
			HNSDatabase_mutex.unlock();
		}
		refresh_pusblish();	

		usleep(1);
	}
    

	return 0;
}

static inline void refresh_pusblish(){
	res_t res;

	for(int i=0; i< HNSDatabase_deviceList_index; i++){
		HNSDatabase_mutex.lock();
		
		res = HNSRefresh_refresh(i);
		if(res == -1) info_error(" ");
		//else printf("success\n");
		res = HNSMQTT_publishDevice(i);

		HNSDatabase_mutex.unlock();
	}
}