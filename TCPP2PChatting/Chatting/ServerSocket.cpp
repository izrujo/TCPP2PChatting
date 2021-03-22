#include "ServerSocket.h"
#include "ClientSocket.h"
#include "Packet.h"

ServerSocket::ServerSocket(Chatter *chatter) {
	this->chatter = chatter;
}

ServerSocket::~ServerSocket() {

}

void ServerSocket::OnAccept(int nErrorCode) {
	ClientSocket *itSocket;
	ClientSocket *newClient = new ClientSocket;

	Packet packet;

	POSITION previous = NULL;
	POSITION current;

	CString packetMessage;
	CString ipAddress;
	CString otherIPAddress;

	UINT portNumber;
	UINT otherPortNumber;

	BOOL onIsSucceed;
	BOOL onIsFind = FALSE;

	onIsSucceed = Accept(*newClient);
	if (onIsSucceed == TRUE) {
		newClient->GetPeerName(ipAddress, portNumber);

		// ��Ŷ�� �����.
		packetMessage.Format("%d:%s:%d", Packet::ID_IP, ipAddress, portNumber);
		packet = Packet((LPCTSTR)packetMessage);

		// ���ο� Ŭ���̾�Ʈ���� ã�ƺ���.
		current = this->clientSockets.GetHeadPosition();
		while (previous != current && current != NULL && onIsFind != TRUE) {// current�� NULL���� Ȯ���ϴ°� ������ؼ� �ٲٱ�.
			itSocket = (ClientSocket*)this->clientSockets.GetAt(current);
			itSocket->GetPeerName(otherIPAddress, otherPortNumber);

			if (ipAddress == otherIPAddress && portNumber == otherPortNumber) {
				onIsFind = TRUE;
			}

			previous = current;
			this->clientSockets.GetNext(current);
		}

		// Ŭ���̾�Ʈ�鿡�� ���ο� Ŭ���̾�Ʈ�� �˷��ش�.
		previous = NULL;
		current = this->clientSockets.GetHeadPosition();
		while (previous != current && current != NULL && onIsFind != TRUE) {
			itSocket = (ClientSocket*)this->clientSockets.GetAt(current);
			itSocket->SendData(&packet);

			previous = current;
			this->clientSockets.GetNext(current);
		}

		// ���ο� Ŭ���̾�Ʈ�̸� �������ְ�, �ƴϸ� ���ش�.
		if (onIsFind != TRUE) {
			newClient->SetServerSocket(this);
			this->clientSockets.AddTail(newClient);

			AfxMessageBox(_T("Other Peer Connected!!"));
		}
		else {
			delete newClient;
		}
	}
	else {
		delete newClient;
		AfxMessageBox(_T("Error: Failed to accept new Client!"));
	}

	CAsyncSocket::OnAccept(nErrorCode);
}

void ServerSocket::CloseClientSocket(ClientSocket* clientSocket) {
	POSITION position;
	position = this->clientSockets.Find(clientSocket); //����Ʈ���� ���� ���� �˻�
	if (position != NULL) {
		if (clientSocket != NULL) {
			clientSocket->ShutDown(); //��� ����
			clientSocket->Close(); //���� �ݱ�
		}

		this->clientSockets.RemoveAt(position); //����Ʈ���� ����
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