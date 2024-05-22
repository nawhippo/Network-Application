#include <Winsock2.h>
#include <iostream>
#include "Packet.h"
#include <cstring>
#include "testfunctions.h"
//packet sniffer should serve as receiver
int SendPacket(ipv4Packet packet){
    //client
    std::cout << "Running Packet Analysis Tests" << std::endl;
    WSADATA wsaData;
    int iResult;

    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0){
        printf("wsa startup failed!");
        return 1;
    }
    //device ips to connect them
    SOCKET transport = socket(AF_INET, SOCK_DGRAM, 0);
    if (transport == INVALID_SOCKET) {
        std::cout << "issue with socket" << std::endl;
            return -1;
    }

    int sockfd;
    
    //if socket file descriptor gives us an error return code. 
    // sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        std::cout << "Socket Creation Error" << std::endl;
        return 1;
    }
    char buffer[1024] = {0};
    SOCKET sockfd;
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    

    //binary conversion of ip address for transport.
    serverAddr.sin_addr.s_addr = packet.destinationAddress
    if (serverAddr.sin_addr.s_addr == INADDR_NONE) {
        std::cerr << "Invalid Address/ address not supported" << std::endl;
        closesocket(sockfd);
        closesocket(transport);
        return -1;
    }


    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0){
        std::cerr << "connection error" << std::endl;
        closesocket(sockfd);
        closesocket(transport);
        return -1;
    }

    //create/send packet
    char buffer[sizeof(packet)];
    std::memcpy(buffer, &packet, sizeof(packet));
    send(transport, buffer, sizeof(buffer), 0);
    


}