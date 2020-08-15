
# Embeddable UDP Server For Mobile Applications
 A simple UDP server that can be embedded within an iOS or Android Application

## What is this for ?

I needed to embed a UDP server, within my cross-platform mobile application (iOS & Android) and was looking for a simple UDP server I could use and customise for my purposes. Most other projects I looked at seemed complicated to setup or understand. I wanted the bare minimum to be able to receive UDP packets, so this project was born.

## How do I use it ?

The server is written in C++. The server code is in UDPServer.h and UDPServer.cpp.

There is a sample implementation of a thread-safe Queue (Queue.h) for storing messages that are received by the server. This sample implementation expects either an ip address (explained later), or a set of four number delimited by semicolons. You can simply modify the queue class for storing your own data structure. The class is intentionally bare-bones / simple do that it is easy to understand.

NetworkHelper.h is some sample code to help determine the ip address that the server is running on. During development I was testing on an iPhone and it was unclear which ip address was being used, as an iPhone appears to have several interfaces. This function gets ip addresses of the interfaces in use.

## Getting Started

First:
```
udpserver_ = new UDPServer(port); //port is an int representing the port you want to try setting up the server on.

udpserver_->setupServer();
```

Then check if the server is ready:

```
udpserver_->isReady()
```

If the server is not ready, then we need to try a different port.

```
udpserver_->nextPort();
```

Then we try to setup the server again until isReady is true.
To get the port being used we do:

```
udpserver_->getPort();
```

It is easy to determine the port we are connected to, but determing the ip address being used by the server on an iphone wasn't straightforward. We had to do two things.

First:

We needed to get a list of the ip addresses for each interface.

Second:

We needed to determine which ip address was the one the UDP server was actually receiving messages on.

To do this, we looped through each ip address and sent a single UDP packed to each address. The content of the packet was the ip address the packet was sent to.

After sending the UDP packets, we keep checking the messages that the UDP server receives. When we receive a message, we inspect the packet contents to determine the IP address in use. We need this along with the port, to tell our senders where to send their UDP packets. When this is done, we call:

```
setFoundIP();
```

This ensures that future messages are not parsed as an ip address, but rather in whatever format we expect to receive them in, and stored in the right queue.

## Receiving messages:

```
udpserver_->getMessage(message)
```
