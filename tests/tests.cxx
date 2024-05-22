#include <assert.h>
#include <Packet.h>
#include <cstring>
#include "testfunctions.h"
int main() {
    //INSTANTIATING A TEST PACKET
    //server should process packet
    //perhaps initalize the packet sender addy with our ip address and the server's ip address

    ipv4Packet packet1(4, 5, 0, 40, 12345, 0, 0, 64, 6, 0, 
                       0xC0A80101, 0xC0A80102, "COOL PACKET DATA FOR TESTING WOWZAH!");

    ipv4Packet packet2(4, 5, 0, 40, 12346, 0, 0, 64, 6, 0, 
                       0xC0A80101, 0xC0A80103, "ANOTHER PACKET DATA FOR TESTING!");

    
    int checksum1 = checksum(packet1);
    int checksum2 = checksum(packet2);
    packet1.checksum = checksum1;
    packet2.checksum = checksum2;
    int result1 = sendpacket(packet1);
    int result2 = sendpacket(packet2);
    assert(1, result1);
    assert(1, result2);
    return 0;
}

int checksum(const ipv4Packet& packet){
    ipv4Packet copy = packet;
    const char * buffer = reinterpret_cast<const char *>(&copy);
    copy.headerChecksum = 0;

    int result = 0;
    for (size_t i = 0; i < sizeof(ipv4Packet); i++){
        if ((i == 6) || (i == 7)){
            continue;
        }
        result += static_cast<unsigned char>(buffer[i]);
    }
    while (result >> 16) {
        result = (result & 0xFFFF) + (result >> 16);
    }
    result = ~result;
    return result & 0xFFFF;
}