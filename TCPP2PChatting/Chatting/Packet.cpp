#include "Packet.h"

Packet::Packet(string identifier, string content) 
	: identifier(identifier), content(content) {

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