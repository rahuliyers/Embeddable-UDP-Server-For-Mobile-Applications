#ifndef NETWORKHELPER_H
#define NETWORKHELPER_H

#include <ifaddrs.h>
#include <arpa/inet.h>

#include <map>
#include <string>

/* A simple method to figure out the IP address on the iPhone that the UDP Server is running on.
There may be more than one interface. Normally if your iPhone is acting as a hotspot via its Wifi, en0 is the interface to look for*/
void getIPAddresses(std::map<std::string, std::string>& addresses) {
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    // Retrieve the current interfaces - returns 0 on success
    int success = getifaddrs(&interfaces);
    if (success == 0) {
        // Loop through linked list of interfaces
        temp_addr = interfaces;
        while (temp_addr != NULL) {
            if (temp_addr->ifa_addr->sa_family == AF_INET) {
                // Check if interface is en0 which is the wifi connection on the iPhone
                addresses[temp_addr->ifa_name] = inet_ntoa(((struct sockaddr_in *)temp_addr->ifa_addr)->sin_addr);
            }
            temp_addr = temp_addr->ifa_next;
        }
    }
    // Free memory
    freeifaddrs(interfaces);
}


#endif /* NETWORKHELPER_H */
