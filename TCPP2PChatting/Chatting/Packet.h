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

	string& GetIdentifier() const;
	string& GetContent() const;

private:
	string identifier;
	string content;
};

inline string& Packet::GetIdentifier() const {
	return const_cast<string&>(this->identifier);
}

inline string& Packet::GetContent() const {
	return const_cast<string&>(this->content);
}

#endif //_PACKET_H