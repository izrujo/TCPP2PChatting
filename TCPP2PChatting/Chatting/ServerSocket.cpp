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

	CString ipAddress;
	CString otherIPAddress;

	UINT portNumber;
	UINT otherPortNumber;

	BOOL onIsSucceed;
	BOOL onIsFind = FALSE;

	onIsSucceed = Accept(*newClient);
	if (onIsSucceed == TRUE) {
		newClient->GetPeerName(ipAddress, portNumber);

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

		previous = NULL;
		current = this->clientSockets.GetHeadPosition();
		while (previous != current && current != NULL) {
			itSocket = (ClientSocket*)this->clientSockets.GetAt(current);
			itSocket->SendData(new Packet(packet));

			previous = current;
			this->clientSockets.GetNext(current);
		}

		if (onIsFind != TRUE) {
			newClient->SetServerSocket(this);
			this->clientSockets.AddTail(newClient);

			ipAddress.Format("%s:%d", (LPCTSTR)ipAddress, portNumber);
			packet = Packet((LPCTSTR)ipAddress, (LPCTSTR)ipAddress);

			AfxMessageBox(_T("Other Peer Connected!!"));
		}

	}
	else {
		delete newClient;
		AfxMessageBox(_T("Error: Failed to accept new Client!"));
	}

	CAsyncSocket::OnAccept(nErrorCode);
}

void ServerSocket::CloseClientSocket(ClientSocket *clientSocket) {
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

void ServerSocket::SendDataAll(Packet *packet) {
	ClientSocket* client = NULL;

	POSITION previous = NULL;
	POSITION current = this->clientSockets.GetHeadPosition();
	while (previous != current && current != NULL) {
		client = (ClientSocket*)this->clientSockets.GetAt(current);
		((ClientSocket*)client)->Send(packet, sizeof(*packet) + lstrlen(packet->GetContent().c_str())); //�����ڵ带 ����ϹǷ� ���ڿ� ���̿� *2
		previous = current;
		this->clientSockets.GetNext(current);
	}
}

void ServerSocket::SendDataAll(TCHAR(*buffer), size_t len) {
	POSITION position;
	position = this->clientSockets.GetHeadPosition();
	ClientSocket* client = NULL;

	while (position != NULL) {
		client = (ClientSocket*)this->clientSockets.GetNext(position);
		if (client != NULL) {
			client->Send(buffer, len); //�����ڵ带 ����ϹǷ� ���ڿ� ���̿� *2
			//��ȯ���� *2�� ���� ���ƾ� �Ѵ�.
		}
	}
}