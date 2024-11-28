/*
    IPAddress: Following code is based on code inside Linux Man Page
    see below

    MAC: source code: https://www.binarytides.com/c-program-to-get-mac-address-from-interface-name-on-linux/
    see below source code (fixed)
*/

#include "HNSHubInfo.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/if_link.h>
#include <string>
#include <string.h>
#include <errno.h>

#include <sys/ioctl.h>
#include <net/if.h>	//ifreq
#include <unistd.h>	//close

//#define INTERFACE   "wlan0" //RaspberryPi Zero W
//#define INTERFACE   "wlo1"  //ASUS

//#define IP_INTERFACE    INTERFACE
//#define MAC_INTERFACE   INTERFACE

#define DEFAULT_IP      "0.0.0.0"
#define DEFAULT_MAC     "0"

//if "interface" is not founded return -1 and set errno=EINVAL (Invalid Argument)
static inline int readIP(const char* interface, std::string& IP);

//if "interface" is not founded return -1 
static inline int readMAC(const char* interface, std::string& MAC);

HNSHubInfo_ HNSHubInfo;


HNSHubInfo_::HNSHubInfo_(){
    this->IPAddress= DEFAULT_IP;
    this->MACAddress=DEFAULT_MAC;
}

int HNSHubInfo_::init(const char* wlan_inteface){
    std::string IP;
    int res = readIP(wlan_inteface, IP);
    if(res==-1){
        perror("Read IP");
        return -1;
    }
    else    this->IPAddress=IP;

    std::string MAC;
    res = readMAC(wlan_inteface, MAC);
    if(res==-1){
        perror("Read MAC");
        return -1;
    }
    else    this->MACAddress=MAC;
    return 0;
}

#define SIZE_BUF    128
std::string HNSHubInfo_::to_string(){
    char buf[SIZE_BUF];
    snprintf(buf, SIZE_BUF, "\tIP %s\n\tMAC %s\n",
                this->IPAddress.c_str(), this->MACAddress.c_str());
    return std::string(buf);
}






static inline int readIP(const char* interface, std::string& IP){
    /*
    lo       AF_PACKET (17)
            tx_packets =       8107; rx_packets =       8107
            tx_bytes   =    1677298; rx_bytes   =    1677298
    eno2     AF_PACKET (17)
            tx_packets =          0; rx_packets =          0
            tx_bytes   =          0; rx_bytes   =          0
    wlo1     AF_PACKET (17)
            tx_packets =      58602; rx_packets =     133091
            tx_bytes   =   12915990; rx_bytes   =  159744480
    lo       AF_INET (2)
            address: <127.0.0.1>
    wlo1     AF_INET (2)
            address: <192.168.178.165>
    lo       AF_INET6 (10)
            address: <::1>
    wlo1     AF_INET6 (10)
            address: <fe80::b72e:6d4f:1e9a:5a27%wlo1>
    */
    struct ifaddrs* ifaddr = NULL;
    struct ifaddrs* ifa = NULL;

    int family, s;
    char host[NI_MAXHOST];

    if (getifaddrs(&ifaddr) == -1) {
        if(ifaddr)  freeifaddrs(ifaddr);
        return -1;
    }

    // Walk through linked list, maintaining head pointer so we can free list later 
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)  continue;
        family = ifa->ifa_addr->sa_family;

        // we want find "interface" IPv4 (not IPv6)
        if(strcmp(ifa->ifa_name, interface) != 0 || family == AF_INET6) continue;
            
    
        // Display interface name and family (including symbolic form of the latter for the common families) 
        /*printf("%-8s %s (%d)\n",
                ifa->ifa_name,
                (family == AF_PACKET) ? "AF_PACKET" :
                (family == AF_INET) ? "AF_INET" :
                (family == AF_INET6) ? "AF_INET6" : "???",
                family);
        */

        // For an AF_INET* interface address, display the address 
        if (family == AF_INET ){//|| family == AF_INET6) {
            s = getnameinfo(ifa->ifa_addr,
                           (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                                 sizeof(struct sockaddr_in6),
                           host, NI_MAXHOST,
                           NULL, 0, NI_NUMERICHOST);
            if (s != 0) {
                freeifaddrs(ifaddr);
                return -1;
            }
            //printf("\t\taddress: <%s>\n", host);

            // find "interface"
            IP=host;
            freeifaddrs(ifaddr);
            return 0;
        } 
        /*
        else if (family == AF_PACKET && ifa->ifa_data != NULL) {
            struct rtnl_link_stats *stats = (rtnl_link_stats *)ifa->ifa_data;
            
            printf("\t\ttx_packets = %10u; rx_packets = %10u\n"
                          "\t\ttx_bytes   = %10u; rx_bytes   = %10u\n",
                          stats->tx_packets, stats->rx_packets,
                          stats->tx_bytes, stats->rx_bytes);
            
        }
        */
    }

    errno = EINVAL; //invalid argument
    freeifaddrs(ifaddr);
    return -1;


    /* 
    //MAIN TEST
    int main(int argc, char *argv[]){
        std::string IP;
        int res = readIP(IP_INTERFACE, IP);
        if(res==-1){
            perror("Read IP");
            return -1;
        }
        else{
            printf("IP=%s\n", IP.c_str());
        }
    }
    */
}

static inline int readMAC(const char* interface, std::string& MAC){
	int fd;
	struct ifreq ifr;

	uint8_t* mac;
	
	fd = socket(AF_INET, SOCK_DGRAM, 0);

	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name , interface , IFNAMSIZ-1);

	int res = ioctl(fd, SIOCGIFHWADDR, &ifr);
	close(fd);
	
	if(res==-1) return -1;
	
	mac = (uint8_t*)ifr.ifr_hwaddr.sa_data;
	
	char buf[16];
	snprintf(buf, 16, "%.2X%.2X%.2X%.2X%.2X%.2X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	MAC = std::string(buf);
	
    return 0;
}







//  IP ADDRESS SOURCE CODE
/*
    ////////// MAN CODE //////////////////////////////
       #define _GNU_SOURCE     // To get defns of NI_MAXSERV and NI_MAXHOST 
       #include <arpa/inet.h>
       #include <sys/socket.h>
       #include <netdb.h>
       #include <ifaddrs.h>
       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h>
       #include <linux/if_link.h>

       int main(int argc, char *argv[])
       {
           struct ifaddrs *ifaddr, *ifa;
           int family, s;
           char host[NI_MAXHOST];

           if (getifaddrs(&ifaddr) == -1) {
               perror("getifaddrs");
               exit(EXIT_FAILURE);
           }

           // Walk through linked list, maintaining head pointer so we
            //  can free list later 

           for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
               if (ifa->ifa_addr == NULL)
                   continue;

               family = ifa->ifa_addr->sa_family;

               // Display interface name and family (including symbolic
               //   form of the latter for the common families) 

               printf("%-8s %s (%d)\n",
                      ifa->ifa_name,
                      (family == AF_PACKET) ? "AF_PACKET" :
                      (family == AF_INET) ? "AF_INET" :
                      (family == AF_INET6) ? "AF_INET6" : "???",
                      family);

               // For an AF_INET* interface address, display the address 

               if (family == AF_INET || family == AF_INET6) {
                   s = getnameinfo(ifa->ifa_addr,
                           (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                                 sizeof(struct sockaddr_in6),
                           host, NI_MAXHOST,
                           NULL, 0, NI_NUMERICHOST);
                   if (s != 0) {
                       printf("getnameinfo() failed: %s\n", gai_strerror(s));
                       exit(EXIT_FAILURE);
                   }

                   printf("\t\taddress: <%s>\n", host);

               } else if (family == AF_PACKET && ifa->ifa_data != NULL) {
                   struct rtnl_link_stats *stats = (rtnl_link_stats *)ifa->ifa_data;

                   printf("\t\ttx_packets = %10u; rx_packets = %10u\n"
                          "\t\ttx_bytes   = %10u; rx_bytes   = %10u\n",
                          stats->tx_packets, stats->rx_packets,
                          stats->tx_bytes, stats->rx_bytes);
               }
           }

           freeifaddrs(ifaddr);
           exit(EXIT_SUCCESS);
       }
*/


//MAC ADDRESS SOURCE CODE
/*

#include <stdio.h>	//printf
#include <string.h>	//strncpy
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>	//ifreq
#include <unistd.h>	//close

int main()
{
	int fd;
	struct ifreq ifr;
	char *iface = "wlo1";//"eth0";
	unsigned char *mac;
	
	fd = socket(AF_INET, SOCK_DGRAM, 0);

	ifr.ifr_addr.sa_family = AF_INET;
	strncpy(ifr.ifr_name , iface , IFNAMSIZ-1);

	ioctl(fd, SIOCGIFHWADDR, &ifr);

	close(fd);
	
	mac = (unsigned char *)ifr.ifr_hwaddr.sa_data;
	
	//display mac address
	printf("Mac : %.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n" , mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	return 0;
}
*/

