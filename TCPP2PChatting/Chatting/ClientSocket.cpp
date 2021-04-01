#include "ClientSocket.h"
#include "ServerSocket.h"
#include "Packet.h"
#include "Chatter.h"
#include "ChattingForm.h"
#include "Viewer.h"
#include "PacketBag.h"

#pragma warning(disable:4996)

ClientSocket::ClientSocket() {
	this->serverSocket = NULL;
}

ClientSocket::ClientSocket(const ClientSocket& source) {
	this->serverSocket = source.serverSocket;
}

ClientSocket::~ClientSocket() {

}

ClientSocket& ClientSocket::operator=(const ClientSocket& source) {
	this->serverSocket = source.serverSocket;

	return *this;
}

void ClientSocket::SetServerSocket(ServerSocket* serverSocket) {
	//1. ���������� �����ϴ�.
	this->serverSocket = serverSocket;
}

void ClientSocket::OnClose(int nErrorCode) {
	//1. ������ �ݴ�.
	CSocket::OnClose(nErrorCode);
	//2. �������Ͽ��� �ݴ�.
	ServerSocket* serverSocket = (ServerSocket*)this->serverSocket;
	serverSocket->CloseClientSocket(this);
}

void ClientSocket::OnReceive(int nErrorCode) {
	ServerSocket* serverSocket = (ServerSocket*)this->serverSocket;

	CString socketAddress;
	UINT socketPortNumber;

	string ipAddress;
	Long portNumber;
	Long number;

	TCHAR buffer[1024];
	//1. ��Ŷ�� �޴�.
	if (this->Receive(buffer, sizeof(buffer)) > 0) {
		Packet* packet = new Packet(buffer);
		Packet::IDENTIFY identifier = packet->GetIdentifier();
		number = packet->GetNumber();
		string content = packet->GetContent();
		//2. ��Ŷ ���濡�� ã��.
		Long index = serverSocket->packetBag->Find(identifier, number);
		//3. ��ã������
		if (index == -1) {
			//3.1. ip�����̸�
			if (identifier == Packet::ID_SYC) {
				// 3.1.1. ��Ŷ���� IP ������ port ������ �д´�.
				packet->GetIPInformations(&ipAddress, &portNumber);
				
				// 3.1.2. Ŭ���̾�Ʈ ������ �����.
				this->serverSocket->chatter->Call(ipAddress, portNumber);
			}
			// 3.2. Ŭ���̾�Ʈ ���� ����ȭ ���� �޽����̸�
			else if (identifier == Packet::ID_SYCACK) {
				//3.2.1. ������ ä�ù濡 �����Ͽ����� �˸��� ä�� ��Ŷ�� �����.
				packet->GetIPInformations(&ipAddress, &portNumber);

				CString comment;
				comment.Format("\r\n\r\n[%s]���� ä�ù濡 �����Ͽ����ϴ�.\r\n\r\n", ipAddress.c_str());
				number = this->serverSocket->packetBag->GetLastNumber(Packet::ID_CHAT_RESPONSE);
				Packet* responsePacket;
				responsePacket = new Packet(number + 1, Packet::ID_CHAT_RESPONSE, (LPCTSTR)comment);
				//3.2.2.��ο��� �����ϴ�.
				this->serverSocket->SendDataAll(responsePacket);
				if (responsePacket != 0) {
					delete responsePacket;
				}
			}
			//3.2. ä���̸�
			else if (identifier == Packet::ID_CHAT_REQUEST || identifier == Packet::ID_CHAT_RESPONSE) {
				//3.2.1. �Ƿ��� ä���̸� ���� �ּҸ� �߰��ϴ�.
				if (identifier == Packet::ID_CHAT_REQUEST) {
					number = this->serverSocket->packetBag->GetLastNumber(Packet::ID_CHAT_RESPONSE);
					this->GetPeerName(socketAddress, socketPortNumber);
					socketAddress.Format("%s:%d : %s", (LPCTSTR)socketAddress, socketPortNumber, content.c_str());
					content = (LPCTSTR)socketAddress;
					if (packet != 0) {
						delete packet;
					}
					packet = new Packet(number + 1, Packet::ID_CHAT_RESPONSE, content);
				}
				//3.2.2. ä�� ������ �����ִ�.
				Viewer viewer(serverSocket->chatter->chattingForm);
				content += "\r\n";
				viewer.View(content);
			}
			//3.3. ��ο��� �����ϴ�.
			serverSocket->SendDataAll(packet);
			//3.4. ��Ŷ ���濡 ��Ŷ�� �߰��ϴ�.
			serverSocket->packetBag->Add(packet);
		}
	}

	CSocket::OnReceive(nErrorCode);
}

void ClientSocket::SendData(Packet* packet) {
	TCHAR(*buffer);
	LONG len;
	//1. �� Ŭ���̾�Ʈ ���ϰ� ����� ������ Ŭ���̾�Ʈ ���Ͽ� �����͸� ������.
	packet->GetPacketMessage(&buffer, &len);

	this->Send(buffer, len);

	if (buffer != 0) {
		delete[] buffer;
	}
}