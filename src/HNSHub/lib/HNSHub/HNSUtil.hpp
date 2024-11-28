#pragma once

#include <iostream>
#include <ctime> 

class HNSUtil{
    private:

    public:
        void printDebug(const char* func_name, const char* msg){
            time_t now = time(NULL); 
            tm* ltm = localtime(&now);
   
            std::cout 
                << "\033[1;33m" 
                << "[DEBUG " << ltm->tm_hour << ":"<< ltm->tm_min << ":" << ltm->tm_sec << "] "
                << "\033[0m" 
                << func_name << ":\t" << msg << std::endl;
        }
        double getTime_us(){
	        struct timespec ts;
	        clock_gettime(CLOCK_MONOTONIC, &ts);
	        return  ts.tv_sec*1E06+ts.tv_nsec*1E-03;
        }
};

extern HNSUtil Utils;
