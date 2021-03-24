#include "ServerSocket.h"
#include "ClientSocket.h"
#include "Packet.h"
#include "PacketBag.h"
#include "Viewer.h"
#include "Chatter.h"

ServerSocket::ServerSocket(Chatter *chatter) {
	this->chatter = chatter;
	this->packetBag = new PacketBag;
}

ServerSocket::ServerSocket(const ServerSocket& source) {
	this->chatter = source.chatter;
	this->packetBag = new PacketBag(*source.packetBag);
}

ServerSocket::~ServerSocket() {
	if (this->packetBag != NULL) {
		delete this->packetBag;
	}
}

ServerSocket& ServerSocket::operator=(const ServerSocket& source) {
	this->chatter = source.chatter;
	if (this->packetBag != 0) {
		delete this->packetBag;
	}
	this->packetBag = new PacketBag(*source.packetBag);

	return *this;
}

void ServerSocket::OnAccept(int nErrorCode) {
	//1. Ŭ���̾�Ʈ ������ �����.
	ClientSocket *newClient = new ClientSocket;
	//2. �޾Ƶ��̴�.
	bool onIsSucceed = Accept(*newClient);
	//3. �޾Ƶ��̴µ� ����������
	if (onIsSucceed == TRUE) {
		//3.1. ip������ ��������.
		CString ipAddress;
		UINT portNumber;
		newClient->GetPeerName(ipAddress, portNumber);
		//3.2. ��Ŷ ���濡�� IP������ ã��.
		string ipInformation;
		Long number = this->packetBag->FindIP(ipInformation);
		//3.3. ��ã������
		if (number == 0) {
			//3.3.1. ��Ŷ�� �����.
			number = this->packetBag->GetLastNumber(Packet::ID_IP);
			CString packetMessage;
			packetMessage.Format("%d:%d:%s:%d", number + 1, Packet::ID_IP, (LPCTSTR)ipAddress, portNumber);
			Packet* packet = new Packet((LPCTSTR)packetMessage);
			//3.3.2. ��ο��� �����ϴ�.
			this->SendDataAll(packet);
			//3.3.3. ����Ʈ�� �߰��ϴ�.
			this->clientSockets.AddTail(newClient);
			//3.3.4. Ŭ���̾�Ʈ ������ ���� ������ �����ϴ�.
			newClient->SetServerSocket(this);
			if (packet != 0) {
				delete packet;
			}

			CString comment;
			comment.Format("\r\n\r\n[%s]���� ä�ù濡 �����Ͽ����ϴ�.\r\n\r\n", ipAddress);
			number = this->packetBag->GetLastNumber(Packet::ID_CHAT_RESPONSE);
			packet = new Packet(number + 1, Packet::ID_CHAT_RESPONSE, (LPCTSTR)comment);
			this->SendDataAll(packet);
			if (packet != 0) {
				delete packet;
			}

			AfxMessageBox(_T("Other Peer Connected!!"));
		}
		else {
			delete newClient;
		}
	}
	//4. ���������� Ŭ���̾�Ʈ ������ ���ִ�.
	else {
		delete newClient;
		AfxMessageBox(_T("Error: Failed to accept new Client!"));
	}

	CAsyncSocket::OnAccept(nErrorCode);
}

void ServerSocket::CloseClientSocket(ClientSocket* clientSocket) {
	POSITION position;
	//1. ����Ʈ���� Ŭ���̾�Ʈ ������ ã��.
	position = this->clientSockets.Find(clientSocket);
	//2. ã������
	if (position != NULL) {
		if (clientSocket != NULL) {
			//2.1. Ŭ���̾�Ʈ ������ ����� �����ϴ�.
			clientSocket->ShutDown(); //��� ����
			//2.2. Ŭ���̾�Ʈ ������ �ݴ�.
			clientSocket->Close(); //���� �ݱ�
		}
		//2.3. Ŭ���̾�Ʈ ������ ����Ʈ���� �����.
		this->clientSockets.RemoveAt(position); //����Ʈ���� ����
		//2.4. Ŭ���̾�Ʈ ������ ���ִ�.
		delete clientSocket; //����
	}
}

void ServerSocket::SendDataAll(Packet* packet) {
	ClientSocket* client = NULL;
	POSITION previous = NULL;
	POSITION current = this->clientSockets.GetHeadPosition();
	while (previous != current && current != NULL) {
		client = (ClientSocket*)this->clientSockets.GetAt(current);
		((ClientSocket*)client)->SendData(packet);
		previous = current;
		this->clientSockets.GetNext(current);
	}
}