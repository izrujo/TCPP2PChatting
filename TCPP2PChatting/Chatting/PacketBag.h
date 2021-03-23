// PacketBag.h
#ifndef _PACKETBAG_H
#define _PACKETBAG_H
#include "..\Utilities\Array.h"
#include "Packet.h"

class PacketBag {
public:
	PacketBag(Long capacity = 32);
	~PacketBag();
	Long Add(Packet* packet);
	Packet* GetAt(Long index);
	Long Find(Packet::IDENTIFY identify, Long number);
	Long FindIP(string ipInformation);
	Long GetLastNumber(Packet::IDENTIFY identify);

public:
	Array<Packet*> packets;
	Long capacity;
	Long length;
};

int CompareIdentifies(void* one, void* other);
int CompareIpInformations(void* one, void* other);

#endif // _PACKETBAG_H