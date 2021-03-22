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

		// 패킷을 만든다.
		packetMessage.Format("%d:%s:%d", Packet::ID_IP, ipAddress, portNumber);
		packet = Packet((LPCTSTR)packetMessage);

		// 새로운 클라이언트인지 찾아본다.
		current = this->clientSockets.GetHeadPosition();
		while (previous != current && current != NULL && onIsFind != TRUE) {// current가 NULL인지 확인하는건 디버깅해서 바꾸기.
			itSocket = (ClientSocket*)this->clientSockets.GetAt(current);
			itSocket->GetPeerName(otherIPAddress, otherPortNumber);

			if (ipAddress == otherIPAddress && portNumber == otherPortNumber) {
				onIsFind = TRUE;
			}

			previous = current;
			this->clientSockets.GetNext(current);
		}

		// 클라이언트들에게 새로운 클라이언트를 알려준다.
		previous = NULL;
		current = this->clientSockets.GetHeadPosition();
		while (previous != current && current != NULL && onIsFind != TRUE) {
			itSocket = (ClientSocket*)this->clientSockets.GetAt(current);
			itSocket->SendData(&packet);

			previous = current;
			this->clientSockets.GetNext(current);
		}

		// 새로운 클라이언트이면 설정해주고, 아니면 없앤다.
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
	position = this->clientSockets.Find(clientSocket); //리스트에서 소켓 선형 검색
	if (position != NULL) {
		if (clientSocket != NULL) {
			clientSocket->ShutDown(); //통신 중지
			clientSocket->Close(); //소켓 닫기
		}

		this->clientSockets.RemoveAt(position); //리스트에서 제거
		delete clientSocket; //삭제
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