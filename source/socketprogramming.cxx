#include <cstring>
#include <iostream>
#include <unistd.h>
#include <Winsock2.h>
#include <stack>
#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
//how to actually monitor all network traffic, surely not through one socket, monitor a list of sockets? list of ip addresses and requests mapped? 
//internet traffic microservice, managed thru docker file, kubernetes

//ping
//request volume
//dropped packets(packets sent out of order, privacy concerns)
//latency (time between requests?)
int main() {
    int routerip = 0;
    if (routerip == 0){
        std::cout << "Please enter router ip info: ";
        std::string userInput;
        std::getline(std::cin, userInput);
        routerip = std::stoi(userInput);
    }
   
    //initialize winsocket
    WSADATA wsaData;
    int iResult;
    //words are 16 bit in winsock (antiquated, but historical term)
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("wsa startup failed: %d\n", iResult);
        return 1;
    }

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

std::string input = NULL;
SOCKET packetInterceptorSOCK = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

if (packetInterceptorSOCK == INVALID_SOCKET) {
    std::cerr << "Socket creation failed :" << WSAGetLastError() << std::endl;
    WSACleanup();
    return 1;
}


while (input.empty()) {
    std::getline(std::cin, input);
    
}
//hypothetically if we wanted to connect to a website we'd use getaddrinfo to get its ip to mess with it, however, since we are using our own ip, we dont have to deal with the dns or url
//monitor host/ network level
//host -> device traffic
//network level -> whole network
//intercepting packets --- how
//define a packets size, data over wifi is sent in binary, we may have to parse it ourselves, probably use a library
//socket made on network router
}

//ipv 4 packets
int analyzePackets() {
SOCKET packetInterceptorSOCK = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
std::queue<char> ThreadQueue;
mutable std::mutex mtx;




SYSTEM_INFO sysinfo;
GetSystemInfo(&sysinfo);
int cores = sysinfo.dwNumberOfProcessors;
std::cout << "Number of Cores: and Threads" << std::endl;

int p = 0;
while(p < cores){
mutex
p = p + 1;
}

}


//input IP Packet from Raw Socket
//Output -1 If no good, checksum fail ???, 1 if good. 
//should be plugged in as thread callable.
int checksumComparison(SOCKET socket, std::queue<char> queue, int count, int droppedcount){
    //of ipv4 length
    char * buffer;
    //it is unnecessary to store ip address info of sender.
    recvfrom(socket, buffer, sizeof(byte) * 65535, 0, NULL, NULL);
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
        unsigned short word = (static_cast<unsigned char>(buffer[i]));
        summation += word;
    }
    //fold 32 to 16
    while(summation >> 16){
        summation = (summation & 0xFFFF) + (summation >> 16);
    }
    unsigned short calculatedChecksum = ~summation;
    //remove from the thread queue

    std::thread call = queue.pop();
    call.join();
    if (summation == calculatedChecksum){
        std::cout << "valid" << std::endl;
        count += 1;
        return 1;
    }
        std::cout << "packet dropped" << std::endl;
        droppedcount += 1;
        return -1;
}






