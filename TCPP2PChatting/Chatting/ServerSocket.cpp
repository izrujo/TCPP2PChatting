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
		if (number == 0) {
			//3.3.1. 패킷을 만들다.
			number = this->packetBag->GetLastNumber(Packet::ID_IP);
			CString packetMessage;
			packetMessage.Format("%d:%d:%s:%d", number + 1, Packet::ID_IP, (LPCTSTR)ipAddress, portNumber);
			Packet* packet = new Packet((LPCTSTR)packetMessage);
			//3.3.2. 모두에게 전달하다.
			this->SendDataAll(packet);
			//3.3.3. 리스트에 추가하다.
			this->clientSockets.AddTail(newClient);
			//3.3.4. 클라이언트 소켓의 서버 소켓을 설정하다.
			newClient->SetServerSocket(this);
			if (packet != 0) {
				delete packet;
			}

			CString comment;
			comment.Format("\r\n\r\n[%s]에서 채팅방에 접속하였습니다.\r\n\r\n", ipAddress);
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