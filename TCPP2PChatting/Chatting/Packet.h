#ifndef _PACKET_H
#define _PACKET_H

#include <string>
using namespace std;

typedef signed long int Long;

class Packet {
public:
	enum IDENTIFY { ID_ERROR = 0, ID_IP = 1, ID_CHAT_REQUEST = 2, ID_CHAT_RESPONSE = 3 };

public:
	Packet(string PacketMessage = "");
	Packet(Long number, IDENTIFY identifier, string content);
	Packet(const Packet& source);
	~Packet();
	Packet& operator=(const Packet& source);

	void GetPacketMessage(char*(*buffer), Long* length);
	void GetIPInformations(string* ipAddress, Long* portNumber);
	Long GetNumber() const;
	IDENTIFY GetIdentifier() const;
	string& GetContent() const;

private:
	Long number;
	IDENTIFY identifier;
	string content;
};

inline Long Packet::GetNumber() const {
	return this->number;
}

inline Packet::IDENTIFY Packet::GetIdentifier() const {
	return this->identifier;
}

inline string& Packet::GetContent() const {
	return const_cast<string&>(this->content);
}

#endif //_PACKET_H