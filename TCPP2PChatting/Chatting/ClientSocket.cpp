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
	//1. 서버소켓을 설정하다.
	this->serverSocket = serverSocket;
}

void ClientSocket::OnClose(int nErrorCode) {
	//1. 소켓을 닫다.
	CSocket::OnClose(nErrorCode);
	//2. 서버소켓에서 닫다.
	ServerSocket* serverSocket = (ServerSocket*)this->serverSocket;
	serverSocket->CloseClientSocket(this);
}

void ClientSocket::OnReceive(int nErrorCode) {
	ServerSocket* serverSocket = (ServerSocket*)this->serverSocket;
	TCHAR buffer[1024];
	//1. 패킷을 받다.
	if (this->Receive(buffer, sizeof(buffer)) > 0) {
		Packet* packet = new Packet(buffer);
		Packet::IDENTIFY identifier = packet->GetIdentifier();
		Long number = packet->GetNumber();
		string content = packet->GetContent();
		//2. 패킷 가방에서 찾다.
		Long index = serverSocket->packetBag->Find(identifier, number);
		//3. 못찾았으면
		if (index == -1) {
			//3.1. 모두에게 전달하다.
			serverSocket->SendDataAll(packet);
			//3.2. ip정보이면 대화자에서 연락하다.
			if (identifier == Packet::ID_IP) {
				//content를 ip주소와 포트번호로 나누기
				string ipAddress;
				Long portNumber;
				packet->GetIPInformations(&ipAddress, &portNumber);
				//CString strTemp = content.c_str();
				//Long punctutation = strTemp.Find(':');
				//ipAddress = strTemp.Left(index); // :의 위치는 0베이스여서 index까지 잘라내면 됨.
				//strTemp = strTemp.Mid(index + 1, strTemp.GetLength() - index - 1);
				//portNumber = _ttoi(strTemp);
				//content를 ip주소와 포트번호로 나누기
				serverSocket->chatter->Call(ipAddress, portNumber);
			}
			//3.3. 채팅이면 패킷 내용을 보여주다.
			else if (identifier == Packet::ID_CHAT) {
				Viewer viewer(serverSocket->chatter->chattingForm);
				viewer.View(content);
			}
			//3.4. 패킷 가방에 패킷을 추가하다.
			serverSocket->packetBag->Add(packet);
		}
	}

	CSocket::OnReceive(nErrorCode);
}

void ClientSocket::SendData(Packet* packet) {
	TCHAR(*buffer);
	LONG len;
	//1. 본 클라이언트 소켓과 연결된 상대방의 클라이언트 소켓에 데이터를 보내다.
	packet->GetPacketMessage(&buffer, &len);

	this->Send(buffer, len);

	if (buffer != 0) {
		delete[] buffer;
	}
}