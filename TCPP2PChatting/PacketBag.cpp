// PacketBag.cpp
/*
파일명칭 : PacketBag.cpp
기능 : 페킷가방 클래스를 만든다.
작성자 : 김찬형, 정성원
작성일자 : 2021.03.23
*/
#include "PacketBag.h"

PacketBag::PacketBag(Long capacity)
	: packets(capacity) {
	this->capacity = capacity;
	this->length = 0;
}

PacketBag::~PacketBag() {

}

Long PacketBag::Add(Packet* packet) {
	Long index;

	if (this->length < this->capacity) {
		index = this->packets.Store(this->length, packet);
	}
	else {
		index = this->packets.AppendFromRear(packet);
		this->capacity++;
	}
	this->length++;

	return index;
}