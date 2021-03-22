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

void ClientSocket::SendData(Packet *packet) {
	this->Send(packet, sizeof(*packet) + lstrlen(packet->GetContent().c_str()) * 2);
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

	Packet *packet = new Packet;
	TCHAR buffer[1024];
	ZeroMemory(buffer, sizeof(buffer));

	Viewer viewer(this->serverSocket->chatter->chattingForm);

	LONG index;

	GetPeerName(ipAddress, portNumber);
	if (Receive(buffer, sizeof(buffer)) > 0) {
		this->serverSocket->SendDataAll(buffer, sizeof(buffer));

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
			strTemp = packet->GetIdentifier().c_str();

			index = strTemp.Find(':', 0);

			ipAddress = strTemp.Left(index);

			strTemp = strTemp.Mid(index, strTemp.GetLength());
			portNumber = _ttoi(strTemp);

			newClientSocket = new ClientSocket;
			newClientSocket->SetServerSocket(this->serverSocket);

			this->serverSocket->clientSockets.AddTail(newClientSocket);
			newClientSocket->Connect(ipAddress, portNumber);

			content.Format("%s가 접속했습니다.\n", packet->GetIdentifier().c_str());
			viewer.View((LPCTSTR)content);
		}
		else {
			viewer.View((LPCTSTR)packet->GetContent().c_str());
		}
	}

	if (packet != 0) {
		delete packet;
	}

	CSocket::OnReceive(nErrorCode);
}

void ClientSocket::OnClose(int nErrorCode) {
	CSocket::OnClose(nErrorCode);

	ServerSocket* serverSocket = (ServerSocket*)this->serverSocket;
	serverSocket->CloseClientSocket(this);
}