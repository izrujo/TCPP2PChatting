// PacketBag.h
#ifndef _PACKETBAG_H
#define _PACKETBAG_H
#include "Utilities/Array.h"
#include "Chatting/Packet.h"

class PacketBag {
public:
	PacketBag(Long capacity = 32);
	~PacketBag();
	Long Add(Packet* packet);
	Packet* GetAt(Long index);
	Long Find(Packet::IDENTIFY identify, Long number);
	Long FindIP(string content);
	Long GetLastNumber(Packet::IDENTIFY identify);

public:
	Array<Packet*> packets;
	Long capacity;
	Long length;
};

#endif // _PACKETBAG_H