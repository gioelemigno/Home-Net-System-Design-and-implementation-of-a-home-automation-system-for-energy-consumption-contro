#pragma once

#include <string>

class HNSHubInfo_{
    private:
        std::string IPAddress;
        std::string MACAddress;

    public:
        HNSHubInfo_();

        //read IP and MAC
        //On success return 0, else -1
        int init(const char* wlan_inteface);

        //get human readable string about HNSHub information
        std::string to_string();

        std::string getIPAddress(){return this->IPAddress;}
        std::string getMACAddress(){return this->MACAddress;}
};

#ifdef __cplusplus
extern "C" {
#endif

extern HNSHubInfo_ HNSHubInfo; 

#ifdef __cplusplus
}
#endif
