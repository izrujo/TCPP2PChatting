// PacketBag.cpp
/*
���ϸ�Ī : PacketBag.cpp
��� : ��Ŷ���� Ŭ������ �����.
�ۼ��� : ������, ������
�ۼ����� : 2021.03.23
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