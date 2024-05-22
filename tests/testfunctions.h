#include "Packet.h"
#include "../source/packetSniffer.cxx"
int SendPacket(ipv4Packet packet);
int checksumComparison(SOCKET socket, std::queue<std::thread> queue, int count, int droppedcount);
int analyzePackets();

