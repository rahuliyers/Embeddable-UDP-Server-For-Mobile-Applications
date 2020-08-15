#include "UDPServer.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

using namespace std;

/**
 * This function parses an incoming message with the following format: 1;234;-89;-53;
 *
 * A valid message consists of 4 integer values separated by semicolons.
 */
inline std::array<int, 4> parseMessage(const std::string& input);
inline std::array<int,4> parseIp(const std::string& input);

UDPServer::UDPServer(unsigned short port)   {
    port_ = port;
}

bool UDPServer::getMessage(std::array<int, 4>& message) {
    return queue_.pop(message);
}

bool UDPServer::getIPAddress(std::array<int, 4>& message) {
    return _ipAddresses.pop(message);
}

void UDPServer::setFoundIP(){
    isFoundIP = true;
}

bool UDPServer::isReady(){
    return _isBoundToPort;
}

void UDPServer::nextPort(){
    port_++;
}

int UDPServer::getPort(){
    return port_;
}

void UDPServer::setupServer() {
    // Launch the server thread.
    std::thread t([this](){
        UDPServerFunc();
    });
    t.detach();
}

int UDPServer::UDPServerFunc() {

    // Creating socket file descriptor
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    CCLOG("Socket created");
    // Filling server information
    struct sockaddr_in servaddr, cliaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    servaddr.sin_family = AF_INET; // IPv

    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port_);

    // Bind the socket with the server address
    if (::bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    _isBoundToPort = true;
    while (true)  {
        // Read the next message from the socket.
        char message[MAXBUFFER_SIZE];
        socklen_t len = sizeof(struct sockaddr);
        ssize_t n = recvfrom(sockfd, (char *)&message, MAXBUFFER_SIZE, MSG_DONTWAIT,
                     (struct sockaddr *)&cliaddr, (socklen_t*)&len);
        if (n > 0) {
            message[n] = '\0';
            // Parse incoming data and push the result on the queue.
            // Parsed messages are represented as a std::array<int, 4>.

            if(!isFoundIP){
                _ipAddresses.push(parseIp(message));
            }else{
                queue_.push(parseMessage(message));
            }
        } else {
            // Wait a fraction of a millisecond for the next message.
            usleep(100);
        }
    }

    return 0;
}

/*A method for getting the numbers in an IP address from a string.
Initially when the UDP server starts, we don't know which IP address it is using.
We only know the port it is bound to.
To figure out the IP address, we send a UDP packet to every IP address that we determine that our iPhone is using.
The UDP packet contains the IP address we sent the packet to.
When we receive the packet, we parse the address to learn which address we are using.
This was necessary, as we coudn't find any documentaion on which interface an iPhone uses when a UDP Server is created.
In the class that uses the UDP server, we check the messages that the server receives.
Once we determine the IP address, we setFoundIP to true, so that future messages are parsed by the parseMessage function.
*/
std::array<int,4> parseIp(const std::string& input){
    std::stringstream ss(input);
    std::array<int, 4> message;
    int n;
    // Loop over all characters in the string and ignore the semicolons.
    for (int i = 0; ss >> n && i < 4; ++i) {
        message[i] = n;
        if (ss.peek() == '.') {
            ss.ignore();
        }
    }
    return message;
}

 void split(const std::string &s, char delim, std::array<int, 4> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    int ctr = 0;
    while (std::getline(ss, item, delim) && ctr<4) {
        std::string::size_type sz;     // alias of size_t
        float value = std::stof (item,&sz);
        elems[ctr++] = (value);
    }
}

std::array<int, 4> parseMessage(const std::string& input) {

    std::array<int, 4> message;
    split(input,';',message);

    return message;
}
