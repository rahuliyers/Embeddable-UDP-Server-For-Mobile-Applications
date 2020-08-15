#ifndef UDPSERVER_H
#define UDPSERVER_H

#include "Queue.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <array>
#include <iostream>
#include <thread>

using namespace std;

#define MAXBUFFER_SIZE 1024

/**
 * A UDP server class.
 */
class UDPServer {
public:
    /**
     * Constructor
     *
     * @port the port on which the UDP server is listening for packets.
     */
    explicit UDPServer(unsigned short port);

    /**
     * Destructor
     */
    ~UDPServer() = default;

    /**
     * Setup the server.
     */
    void setupServer();

    /**
     * Get a single message.
     * For demonstration purposes, our messages is expected to be a array of int
     */
    bool getMessage(std::array<int, 4>& message);

    bool getIPAddress(std::array<int, 4>& message);

    void setFoundIP();

    bool isReady();

    void nextPort();

    int getPort();

private:
    bool _isBoundToPort = false;
    /**
     * The server port.
     */
    unsigned short port_;
    bool isFoundIP = false;
    /**
     * The thread-safe message queue.
     */
    Queue queue_;
    Queue _ipAddresses;
    /**
     * The UDP server function.
     */
    int UDPServerFunc();
};

#endif
