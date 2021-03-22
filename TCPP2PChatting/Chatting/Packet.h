#ifndef _PACKET_H
#define _PACKET_H

#include <string>
using namespace std;

typedef signed long int Long;

class Packet {
public:
	enum IDENTIFY { ID_ERROR = 0, ID_IP = 1, ID_CHAT = 2 };

public:
	Packet(string PacketMessage = "");
	Packet(IDENTIFY identifier, string content);
	Packet(const Packet& source);
	~Packet();
	Packet& operator=(const Packet& source);

	void GetPacketMessage(char*(*buffer), Long* length);
	IDENTIFY GetIdentifier() const;
	string& GetContent() const;

private:
	IDENTIFY identifier;
	string content;
};

inline Packet::IDENTIFY Packet::GetIdentifier() const {
	return this->identifier;
}

inline string& Packet::GetContent() const {
	return const_cast<string&>(this->content);
}

#endif //_PACKET_H