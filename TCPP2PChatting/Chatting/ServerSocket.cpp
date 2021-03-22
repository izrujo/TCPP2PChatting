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
		while (previous != current && current != NULL && onIsFind != TRUE) {// current가 NULL인지 확인하는건 디버깅해서 바꾸기.
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

void ServerSocket::SendDataAll(Packet *packet) {
	ClientSocket* client = NULL;

	POSITION previous = NULL;
	POSITION current = this->clientSockets.GetHeadPosition();
	while (previous != current && current != NULL) {
		client = (ClientSocket*)this->clientSockets.GetAt(current);
		((ClientSocket*)client)->Send(packet, sizeof(*packet) + lstrlen(packet->GetContent().c_str())); //유니코드를 사용하므로 문자열 길이에 *2
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
			client->Send(buffer, len); //유니코드를 사용하므로 문자열 길이에 *2
			//반환값은 *2한 값과 같아야 한다.
		}
	}
}