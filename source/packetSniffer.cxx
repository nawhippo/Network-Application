#include <cstring>
#include <iostream>
#include <unistd.h>
#include <Winsock2.h>
#include <stack>
#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include "../tests/testfunctions.h"
//how to actually monitor all network traffic, surely not through one socket, monitor a list of sockets? list of ip addresses and requests mapped? 
//internet traffic microservice, managed thru docker file, kubernetes

//ping
//request volume
//dropped packets(packets sent out of order, privacy concerns)
//latency (time between requests?)
int main() {
SOCKET packetInterceptorSOCK; 
if (initializeWinsockAndCreateSocket(packetInterceptorSOCK) != 0){
    return 1;
}
std::unordered_map<std::string, int> ipAddresses;
//irrelevant for intranetwork communication
// ----------------------------------------
//     struct addrinfo {
//     addrinfo* result = NULL;
//     addrinfo* ptr = NULL;
//     addrinfo* hints;
// }
// addrinfo myinfo = NULL;
// ZeroMemory( &hints, sizeof(hints) );
// hints.ai_family = AF_INET;
// hints.ai_socktype = SOCK_STREAM;
// hints.ai_protocol = IPPROTO_TCP;
// ----------------------------------------

//raw sockets allow us to interface with underlying packet transport.
//we'd need to examine whether a packet has been dropped, presumably by check sequence numbers etc
//ip level packet retransmision is handled above our level. 
//efficient analysis required for large ip packet storage

//allow users to interrupt real time packet analysis
std::cout << "press any input to interrupt packet analysis" <<std::endl;
std::string input;
std::thread analysisThread(startPacketAnalysis, ref(packetInterceptorSOCK));
std::getline(std::cin, input);
analysisThread.detach();
closesocket(packetInterceptorSOCK);
WSACleanup();
return 0;
}
//hypothetically if we wanted to connect to a website we'd use getaddrinfo to get its ip to mess with it, however, since we are using our own ip, we dont have to deal with the dns or url
//monitor host/ network level
//host -> device traffic
//network level -> whole network
//intercepting packets --- how
//define a packets size, data over wifi is sent in binary, we may have to parse it ourselves, probably use a library
//socket made on network router

int init(Socket &packetInterceptorSOCK) {
    WSAData wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if  (iResult != 0) {
        std::cerr << "WSA Startup Failed" << std::endl;
        return 1;
    }
    packetInterceptorSOCK = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (packetInterceptorSOCK == INVALID_SOCKET) {
        std::cerr << "Socket Creation Failed. " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    return 0;
}

void startPacketAnalysis(SOCKET packetInterceptorSOCK){
    analyzePackets(packetInterceptorSOCK);
}
//ipv 4 packets
int analyzePackets(Socket packetInterceptorSOCK) {
std::queue<std::thread> ThreadQueue;
std::mutex mtx;
SYSTEM_INFO sysinfo;
GetSystemInfo(&sysinfo);
int cores = std::thread::hardware_concurrency();
int p = 0;
int droppedcount = 0;
int count = 0;
for (i = 0; i < cores; i++){
//pass mutex and multithread the checksum comparison
ThreadQueue.push(std::thread(checksumComparison, packetInterceptorSOCK, std::ref(ThreadQueue), std::ref(count), std::ref(droppedcount)));
}
//pop then join current thread
while(!ThreadQueue.empty()){
    ThreadQueue.front().join();
    ThreadQueue.pop();
}
}

int extractDestination(ipv4Packet packet, ref(ipAddresses)){
    unsigned int ipAddress = packet.destinationAddress;
    if (ipAddresses.find(ipAddress) == ipAddresses.end()){
        ipAddresses[ipAddress] = 1;
    } else {
        ipAddresses[ipAddress] += 1;
    }    
}

int printIpAddressTraffic(ref(ipAddresses)){
    for(int i = 0; i < sizeof(ipAddresses); i++){
    printf("%25d : %25d packet(s) incoming \n", ipAddress[i].first, ipAddresses[i].second);
}
return 0; 
}
//input IP Packet from Raw Socket
//Output -1 If no good, checksum fail ???, 1 if good. 
//should be plugged in as thread callable.
int checksumComparison(SOCKET socket, std::queue<std::thread> queue, int count, int droppedcount){
    //of ipv4 length
    char buffer[65535];
    //it is unnecessary to store ip address info of sender.
    int BytesRecieved = recvfrom(socket, buffer, sizeof(byte) * 65535, 0, NULL, NULL);
    if (bytesRecieved == SOCKET_ERROR){
        std::cerr << "recvfrom error" << std::endl;
        return -1;
    }
    std::string checksum;
    //bytes six and seven are where the checksum is 
    checksum += buffer[6];
    checksum += buffer[7];
    int check = stoi(checksum);
    //now loop through the buffer entirely to see if checksum matches
    int summation = 0;
    for(size_t i = 0; i < strlen(buffer); ++i=2){
        //don't count the checksum itself. 
        if (i == 6){
            continue;
        }
        unsigned short word = static_cast<unsigned char>(buffer[i]);
        summation += word;
    }
    //fold 32 to 16
    while(summation >> 16){
        summation = (summation & 0xFFFF) + (summation >> 16);
    }
    unsigned short calculatedChecksum = ~summation;
    //remove from the thread queue
    queue.pop();
    if (summation == calculatedChecksum){
        std::cout << "valid" << std::endl;
        count += 1;
        return 1;
    }
        std::cout << "packet dropped" << std::endl;
        droppedcount += 1;
        return -1;
}






