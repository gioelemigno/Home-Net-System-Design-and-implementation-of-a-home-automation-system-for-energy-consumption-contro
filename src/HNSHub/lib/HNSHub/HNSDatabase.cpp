#ifdef __linux__
#include "HNSDatabase.hpp"
#include "HNSDevice_class.hpp"
#include "../HNSCore/HNSMacro.h"

#include "../HNSHubLib/HNSSocketPower/HNSSocketPower.h"

#include <list>

#include "HNSAddressMap.hpp"

std::mutex HNSDatabase_mutex;
std::list<address_t> freeAddressList;

const char* HNSHub_IP;
const char* HNSHub_MAC;

//HNSDevice* HNSDatabase_device[MAX_N_DEVICE];
//uint16_t HNSDatabase_device_index=0; //indicate new address available

uint16_t HNSDatabase_deviceList_index=0; //indicate new address available
HNSDatabase_entry HNSDatabase_deviceList[MAX_N_DEVICE]; //auto set to zero (stored in BSS segment)

res_t HNSDatabase_getNewAddress(address_t* address){
  if(! freeAddressList.empty()){
    *address = freeAddressList.front();
  }
  else{
    if(HNSDatabase_deviceList_index < MAX_N_DEVICE){
      *address = HNSDatabase_deviceList_index;
    }
    else{
      error=ERR_NO_FREE_ADDRESS;
      return -1;
    }
  }

  return 0;
}

res_t HNSDatabase_init(const char* _HNSHub_IP, const char* _HNSHub_MAC){
  HNSHub_IP = _HNSHub_IP;
  HNSHub_MAC = _HNSHub_MAC;
  return 0;
}

res_t HNSDatabase_add2(uint16_t model, address_t* address, uint16_t HNS_uid){
    if(! freeAddressList.empty()){
      *address = freeAddressList.front();
    }
    else{
      if(HNSDatabase_deviceList_index < MAX_N_DEVICE){
        *address = HNSDatabase_deviceList_index;
      }
      else{
        error=ERR_NO_FREE_ADDRESS;
        return -1;
      }
    }

    address_t index = *address;
    HNSDevice* newDevice;

    switch(model){
      case HNSModel_HNSTemperatureHumiditySensor:
     //   newDevice=new HNSTemperatureHumiditySensor(index);
      break;

      case HNSModel_HNSLight:
     //   newDevice=new HNSLight(index);
      break;

      case HNSModel_HNSSocket:
   //     newDevice=new HNSSocket(index);
      break;

      case HNSModel_HNSSocketPower:
        //newDevice = new HNSSocketPower(index, HNS_uid);
        //HNSSocketPower(address_t address, uint32_t HNSCodeID, const char* HNSHub_MAC, const char* HNSHub_IP);

        newDevice = new HNSSocketPower(index, HNS_uid, HNSHub_MAC, HNSHub_IP);
      break;
      
      default:
        error=ERR_UNKNOWN_MODEL;
        return -1;
      break;
    }
    HNSDatabase_deviceList[index].device=newDevice;
    HNSDatabase_deviceList[index].status=deviceStatus_active;

    if(! freeAddressList.empty()) freeAddressList.pop_front();
    else HNSDatabase_deviceList_index++;

    //add in address map
    //HNSAddressMap_add(newDevice->getDeviceName(), newDevice->getAddress());
    
    newDevice->init();
    //std::string test = newDevice->getMQTTDeviceID() + std::string("_") + std::to_string(index);
    //HNSAddressMap_add(test, newDevice->getAddress());
    HNSAddressMap_add(newDevice->getMQTTDeviceID(), newDevice->getAddress());

    return 0;
}

//on success return address
res_t HNSDatabase_add(uint16_t model, address_t* address){

    if(! freeAddressList.empty()){
      *address = freeAddressList.front();
    }
    else{
      if(HNSDatabase_deviceList_index < MAX_N_DEVICE){
        *address = HNSDatabase_deviceList_index;
      }
      else{
        error=ERR_NO_FREE_ADDRESS;
        return -1;
      }
    }

    address_t index = *address;
    HNSDevice* newDevice;

    switch(model){
      case HNSModel_HNSTemperatureHumiditySensor:
     //   newDevice=new HNSTemperatureHumiditySensor(index);
      break;

      case HNSModel_HNSLight:
     //   newDevice=new HNSLight(index);
      break;

      case HNSModel_HNSSocket:
   //     newDevice=new HNSSocket(index);
      break;

      case HNSModel_HNSSocketPower:
        newDevice = new HNSSocketPower(index);
      break;
      
      default:
        error=ERR_UNKNOWN_MODEL;
        return -1;
      break;
    }
    HNSDatabase_deviceList[index].device=newDevice;
    HNSDatabase_deviceList[index].status=deviceStatus_active;

    if(! freeAddressList.empty()) freeAddressList.pop_front();
    else HNSDatabase_deviceList_index++;

    //add in address map
    //HNSAddressMap_add(newDevice->getDeviceName(), newDevice->getAddress());
    
    newDevice->init();
    //std::string test = newDevice->getMQTTDeviceID() + std::string("_") + std::to_string(index);
    //HNSAddressMap_add(test, newDevice->getAddress());
    HNSAddressMap_add(newDevice->getMQTTDeviceID(), newDevice->getAddress());

    return 0;
}

res_t HNSDatabase_remove(address_t address_to_remove){
  std::string name = HNSDatabase_deviceList[address_to_remove].device->getDeviceName();
  HNSAddressMap_remove(name);
  
  HNSDatabase_deviceList[address_to_remove].status=deviceStatus_deleted;
  freeAddressList.push_back(address_to_remove);
  return 0;
}

#endif /*__linux__*/
