#include "Packet.h"

Packet::Packet(string packetMessage) {
	Long index;
	index = packetMessage.find(':');

	this->identifier = ID_ERROR;
	if (index != string::npos) {
		this->identifier = (IDENTIFY)stoi(packetMessage.substr(0, index));
		this->content = packetMessage.substr(index + 1, packetMessage.length() - index -1);
	}
}

Packet::Packet(IDENTIFY identifier, string content)
	: content(content) {
	this->identifier = identifier;
}

Packet::Packet(const Packet& source) 
	: identifier(source.identifier), content(source.content) {

}

Packet::~Packet() {

}

Packet& Packet::operator=(const Packet& source) {
	this->identifier = source.identifier;
	this->content = source.content;

	return *this;
}

void Packet::GetPacketMessage(char*(*buffer), Long* length) {
	string tempString;

	tempString = to_string(this->identifier);

	tempString += ":" + this->content;

	*length = tempString.length();
	*buffer = new char[*length];

	memset(*buffer, *length, sizeof(char));

	strcpy(*buffer, tempString.c_str());
}