#include "ClientSocket.h"
#include "ServerSocket.h"
#include "Packet.h"
#include "Chatter.h"
#include "ChattingForm.h"
#include "Viewer.h"

#pragma warning(disable:4996)
ClientSocket::ClientSocket(){
	this->serverSocket = NULL;
}

ClientSocket::~ClientSocket(){

}

void ClientSocket::SetServerSocket(ServerSocket* serverSocket){
	this->serverSocket = serverSocket;
}

void ClientSocket::SendData(Packet* packet) {
	TCHAR(*buffer);
	LONG len;

	packet->GetPacketMessage(&buffer, &len);

	this->Send(buffer, len);

	if (buffer != 0) {
		delete[] buffer;
	}
}

void ClientSocket::OnReceive(int nErrorCode){
	CString content;
	CString strTemp;
	CString ipAddress;
	CString otherIPAddress;

	UINT portNumber;
	UINT otherPortNumber;

	BOOL onIsFind = FALSE;

	POSITION previous = NULL;
	POSITION current;

	ClientSocket *newClientSocket;
	ClientSocket *itSocket;

	Packet packet;

	TCHAR buffer[1024];

	LONG len;
	LONG index;

	GetPeerName(ipAddress, portNumber);
	if (Receive(buffer, sizeof(buffer)) > 0) {
		packet = Packet(buffer);
		this->serverSocket->SendDataAll(&packet);

		current = this->serverSocket->clientSockets.GetHeadPosition();
		while (previous != current && current != NULL && onIsFind != TRUE) {
			itSocket = (ClientSocket*)this->serverSocket->clientSockets.GetAt(current);
			itSocket->GetPeerName(otherIPAddress, otherPortNumber);
			
			if (ipAddress == otherIPAddress && portNumber == otherPortNumber) {
				onIsFind = TRUE;
			}

			previous = current;
			this->serverSocket->clientSockets.GetNext(current);
		}

		if (onIsFind != TRUE) {
			strTemp = packet.GetContent().c_str();

			index = strTemp.Find(':');

			ipAddress = strTemp.Left(index); // :의 위치는 0베이스여서 index까지 잘라내면 됨.

			strTemp = strTemp.Mid(index + 1, strTemp.GetLength() - index - 1);
			portNumber = _ttoi(strTemp);

			newClientSocket = new ClientSocket;
			newClientSocket->SetServerSocket(this->serverSocket);

			this->serverSocket->clientSockets.AddTail(newClientSocket);
			newClientSocket->Connect(ipAddress, portNumber);
		}
	}

	CSocket::OnReceive(nErrorCode);
}

void ClientSocket::OnClose(int nErrorCode) {
	CSocket::OnClose(nErrorCode);

	ServerSocket* serverSocket = (ServerSocket*)this->serverSocket;
	serverSocket->CloseClientSocket(this);
}