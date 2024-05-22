//Packet.h
//for use in testing sending packets.
#ifndef PACKET_H
#define PACKET_H
#include <cstring>
#include <cstdint>
struct ipv4Packet {
    unsigned int version : 4;
    unsigned int headerLength : 4;
    //out of date w/ modern qos standards. 
    unsigned int typeOfService : 8;
    unsigned int totalLength : 16;
    unsigned int identification : 16;
    unsigned int flags : 3;
    unsigned int fragmentOffset: 13;
    //hops actually(not even time related)
    unsigned int timeToLive : 8;
    unsigned int protocol : 8;
    unsigned int headerChecksum : 16;
    unsigned int sourceAddress;
    unsigned int destinationAddress;
    char data[1024];

    ipv4Packet(uint8_t ver, uint8_t hdrLen, uint8_t tos, uint16_t totLen, uint16_t id, 
               uint8_t flgs, uint16_t fragOff, uint8_t ttl, uint8_t proto, uint16_t hdrChksum,
               uint32_t srcAddr, uint32_t destAddr, const char* payload) :
        version(ver), headerLength(hdrLen), typeOfService(tos), totalLength(totLen), 
        identification(id), flags(flgs), fragmentOffset(fragOff), timeToLive(ttl), 
        protocol(proto), headerChecksum(hdrChksum), sourceAddress(srcAddr), 
        destinationAddress(destAddr) {
        std::memset(data, 0, sizeof(data)); // Clear data buffer
        std::strncpy(data, payload, sizeof(data) - 1); // Copy payload
    }
};

struct ipv6Packet {
    unsigned int version : 4;
    unsigned int trafficClass : 4;
    unsigned int flowlabel : 20;
    unsigned int payloadLength : 16;
    unsigned int nextHeader : 8;
    unsigned int hopLimit : 8;
    unsigned int sourceAddress[16];
    unsigned int destinationAddress[16];
    char data[1024];

    ipv6Packet(uint8_t ver, uint8_t tClass, uint32_t fLabel, uint16_t pLen, uint8_t nHeader, 
               uint8_t hLimit, const unsigned char srcAddr[16], const unsigned char destAddr[16], 
               const char* payload) :
        version(ver), trafficClass(tClass), flowLabel(fLabel), payloadLength(pLen), 
        nextHeader(nHeader), hopLimit(hLimit) {
        std::memcpy(sourceAddress, srcAddr, 16); 
        std::memcpy(destinationAddress, destAddr, 16); 
        std::memset(data, 0, sizeof(data)); 
        std::strncpy(data, payload, sizeof(data) - 1); 
    }
};

#endif