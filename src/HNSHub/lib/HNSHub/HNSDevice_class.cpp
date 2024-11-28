#pragma once 

#include "HNSDevice_class.hpp"
#include <stdint.h>
#include <string>
#include <time.h>



res_t HNSDevice::refresh(){
  printf("Error: no refresh for HNSDevice base class\n");
  return -1;
}

res_t HNSDevice::init(){
  printf("Error: no init() for HNSDevice base class\n");
  return -1;
}

std::string HNSDevice::toString(){
  std::string str = "Hi i'm HNSDev base\n";
  return str;
}

res_t HNSDevice::setCommand(MQTTCommand_struct& command){
  return -1;
}

