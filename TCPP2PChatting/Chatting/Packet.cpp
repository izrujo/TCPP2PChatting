#include "Packet.h"

#pragma warning(disable:4996)

Packet::Packet(string packetMessage) {
	Long offset = 0;
	Long index;
	index = packetMessage.find(':', offset);

	this->identifier = ID_ERROR;
	if (index != string::npos) {
		this->number = stoi(packetMessage.substr(0, index));

		offset = index + 1;
		index = packetMessage.find(':', offset);
		if (index != string::npos) {
			this->identifier = (IDENTIFY)stoi(packetMessage.substr(offset, index - offset));

			offset = index + 1;
			this->content = packetMessage.substr(offset, packetMessage.length() - offset);
		}
	}
}

Packet::Packet(Long number, IDENTIFY identifier, string content)
	: content(content) {
	this->number = number;
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

	tempString = to_string(this->number);

	tempString += ":" + to_string(this->identifier);

	tempString += ":" + this->content;

	*length = tempString.length()+1;
	*buffer = new char[*length];

	memset(*buffer, *length, sizeof(char));

	strcpy(*buffer, tempString.c_str());
}

void Packet::GetIPInformations(string* ipAddress, Long* portNumber) {
	Long offset = 0;
	Long index;
	
	index = this->content.find(':', offset);
	if (index != string::npos) {
		*ipAddress = this->content.substr(offset, index);

		offset = index + 1;
		*portNumber = stoi(this->content.substr(offset, this->content.length() - offset));
	}
}