#include "ServerSocket.h"
#include "ClientSocket.h"
#include "Packet.h"
#include "PacketBag.h"

ServerSocket::ServerSocket(Chatter *chatter) 
	: packetBag() {
	this->chatter = chatter;
}

ServerSocket::ServerSocket(const ServerSocket& source) 
	: chatter(source.chatter), packetBag(source.packetBag) {

}

ServerSocket::~ServerSocket() {
	if (this->packetBag != NULL) {
		delete this->packetBag;
	}
}

void ServerSocket::OnAccept(int nErrorCode) {
	//1. 클라이언트 소켓을 만들다.
	ClientSocket *newClient = new ClientSocket;
	//2. 받아들이다.
	bool onIsSucceed = Accept(*newClient);
	//3. 받아들이는데 성공했으면
	if (onIsSucceed == TRUE) {
		//3.1. ip정보를 가져오다.
		CString ipAddress;
		UINT portNumber;
		newClient->GetPeerName(ipAddress, portNumber);
		//3.2. 패킷 가방에서 IP정보를 찾다.
		string ipInformation;
		Long number = this->packetBag->FindIP(ipInformation);
		//3.3. 못찾았으면
		if (number == -1) {
			//3.3.1. 패킷을 만들다.
			CString packetMessage;
			packetMessage.Format("%d:%s:%d", Packet::ID_IP, ipAddress, portNumber);
			Packet* packet = new Packet((LPCTSTR)packetMessage);
			//3.3.2. 모두에게 전달하다.
			this->SendDataAll(packet);
			//3.3.3. 리스트에 추가하다.
			this->clientSockets.AddTail(newClient);
			//3.3.4. 클라이언트 소켓의 서버 소켓을 설정하다.
			newClient->SetServerSocket(this);

			AfxMessageBox(_T("Other Peer Connected!!"));
		}
		else {
			delete newClient;
		}
	}
	//4. 실패했으면 클라이언트 소켓을 없애다.
	else {
		delete newClient;
		AfxMessageBox(_T("Error: Failed to accept new Client!"));
	}

	CAsyncSocket::OnAccept(nErrorCode);
}

void ServerSocket::CloseClientSocket(ClientSocket* clientSocket) {
	POSITION position;
	//1. 리스트에서 클라이언트 소켓을 찾다.
	position = this->clientSockets.Find(clientSocket);
	//2. 찾았으면
	if (position != NULL) {
		if (clientSocket != NULL) {
			//2.1. 클라이언트 소켓의 통신을 중지하다.
			clientSocket->ShutDown(); //통신 중지
			//2.2. 클라이언트 소켓을 닫다.
			clientSocket->Close(); //소켓 닫기
		}
		//2.3. 클라이언트 소켓을 리스트에서 지우다.
		this->clientSockets.RemoveAt(position); //리스트에서 제거
		//2.4. 클라이언트 소켓을 없애다.
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