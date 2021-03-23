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

Packet* PacketBag::GetAt(Long index) {
	return this->packets.GetAt(index);
}

Long PacketBag::Find(Packet::IDENTIFY identify, Long number) {
	Long index = -1;
	Long(*identifiers);
	Long identyCount;
	Long otherNumber;
	Long i = 0;

	Packet* packet;

	this->packets.LinearSearchDuplicate(&identify, &identifiers, &identyCount, CompareIdentifies);

	while (i < identyCount && index <= -1) {
		packet = this->packets.GetAt(identifiers[i]);

		otherNumber = packet->GetNumber();
		if (number == otherNumber) {
			index = identifiers[i];
		}
		i++;
	}

	if (identifiers != 0) {
		delete identifiers;
	}

	return index;
}

Long PacketBag::FindIP(string ipInformation) {
	Long number = 0;
	Long index;

	Packet* packet;

	index = this->packets.LinearSearchUnique(&ipInformation, CompareIpInformations);
	
	if (index > -1) {
		packet = this->packets.GetAt(index);

		number = packet->GetNumber();
	}

	return number;
}

Long PacketBag::GetLastNumber(Packet::IDENTIFY identify) {
	Packet* packet;

	Packet::IDENTIFY otherIdentify;

	Long number = 0;
	Long i = this->length - 1;

	while (i >= 0 && number <= 0) {
		packet = this->packets.GetAt(i);

		otherIdentify = packet->GetIdentifier();
		if (identify == otherIdentify) {
			number = packet->GetNumber();
		}
		i++;
	}

	return number;
}

int CompareIdentifies(void* one, void* other) {
	Long ret;

	Packet* one_ = (Packet*)one;
	Packet::IDENTIFY* other_ = (Packet::IDENTIFY*)other;

	if (one_->GetIdentifier() < *other_) {
		ret = -1;
	}
	else if (one_->GetIdentifier() == *other_) {
		ret = 0;
	}
	else if (one_->GetIdentifier() > *other_) {
		ret = 1;
	}

	return ret;
}

int CompareIpInformations(void* one, void* other) {
	Packet* one_ = (Packet*)one;
	string* other_ = (string*)other;

	return one_->GetContent().compare(other_->c_str());
}