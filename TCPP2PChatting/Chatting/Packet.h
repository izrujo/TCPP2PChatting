#ifndef _PACKET_H
#define _PACKET_H

#include <string>

using namespace std;

class Packet {
public:
	Packet(string identifier = "", string content = "");
	Packet(const Packet& source);
	~Packet();
	Packet& operator=(const Packet& source);

private:
	string identifier;
	string content;
};

#endif //_PACKET_H