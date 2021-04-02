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
	this->serverSocket->CloseClientSocket(this);
}

void ClientSocket::OnReceive(int nErrorCode) {
	ServerSocket* serverSocket = (ServerSocket*)this->serverSocket;

	CString socketAddress;
	UINT socketPortNumber;

	CString otherIPAddress;
	UINT otherPortNumber;

	string ipAddress;
	Long portNumber;
	Long number;

	TCHAR buffer[1024];
	//1. 패킷을 받다.
	if (this->Receive(buffer, sizeof(buffer)) > 0) {
		Packet* packet = new Packet(buffer);
		Packet::IDENTIFY identifier = packet->GetIdentifier();
		number = packet->GetNumber();
		string content = packet->GetContent();
		//2. 패킷 가방에서 찾다.
		Long index = serverSocket->packetBag->Find(identifier, number);
		//3. 못찾았으면
		if (index == -1) {
			if (identifier == Packet::ID_SYCMY) {
				// 자신의 ip 정보를 설정한다.
				packet->GetIPInformations(&ipAddress, &portNumber);
				this->serverSocket->ipAddress = ipAddress.c_str();
				this->serverSocket->portNumber = portNumber;
			}
			//3.1. ip정보이면
			if (identifier == Packet::ID_SYC) {
				// 3.1.1. 패킷에서 IP 정보와 port 정보를 읽는다.
				packet->GetIPInformations(&ipAddress, &portNumber);
				
				// 3.1.2. 클라이언트 소켓을 만든다.
				this->serverSocket->chatter->Call(ipAddress, portNumber);
			}
			// 3.2. 클라이언트 정보 동기화 성공 메시지이면
			else if (identifier == Packet::ID_SYCACK) {
				//3.2.1. 누군가 채팅방에 접속하였음을 알리는 채팅 패킷을 만들다.
				packet->GetIPInformations(&ipAddress, &portNumber);

				CString comment;
				comment.Format("\r\n\r\n[%s]에서 채팅방에 접속하였습니다.\r\n\r\n", ipAddress.c_str());
				number = this->serverSocket->packetBag->GetLastNumber(Packet::ID_CHAT_RESPONSE);
				Packet* responsePacket;
				responsePacket = new Packet(number + 1, Packet::ID_CHAT_RESPONSE, (LPCTSTR)comment);
				//3.2.2.모두에게 전달하다.
				this->serverSocket->SendDataAll(responsePacket);
				if (responsePacket != 0) {
					delete responsePacket;
				}
			}
			// 3.3. 종료 메시지이면
			else if (identifier == Packet::ID_FINISH) {
				// 3.3.1. 종료를 요청한 클라이언트를 찾는다.
				packet->GetIPInformations(&ipAddress, &portNumber);

				BOOL onIsFind = FALSE;
				ClientSocket* client = NULL;
				POSITION previous = NULL;
				POSITION current = this->serverSocket->clientSockets.GetHeadPosition();
				while (previous != current && current != NULL && onIsFind != TRUE) {
					client = (ClientSocket*)this->serverSocket->clientSockets.GetAt(current);
					
					client->GetPeerName(otherIPAddress, otherPortNumber);
					if (ipAddress.compare((LPCTSTR)otherIPAddress) == 0 && portNumber == otherPortNumber) {
						onIsFind = TRUE;
					}

					previous = current;
					this->serverSocket->clientSockets.GetNext(current);
				}

				// 3.3.2. 종료 허가 패킷을 만든다.
				Packet* finishPacket = new Packet(0, Packet::ID_FINISHACK, "");

				if (onIsFind == TRUE) {
					client->SendData(finishPacket);
				}

				if (finishPacket != 0) {
					delete finishPacket;
				}
			}
			// 3.4. 종료 허가 메시지이면
			else if (identifier == Packet::ID_FINISHACK) {
				// 3.4.1. 종료한다.
				this->serverSocket->chatter->onIsFinished = TRUE;

				this->serverSocket->chatter->chattingForm->PostMessage(WM_CLOSE);
				this->serverSocket->CloseClientSocket(this);
			}
			//3.5. 채팅이면
			else if (identifier == Packet::ID_CHAT_REQUEST || identifier == Packet::ID_CHAT_RESPONSE) {
				//3.5.1. 의뢰한 채팅이면 소켓 주소를 추가하다.
				if (identifier == Packet::ID_CHAT_REQUEST) {
					number = this->serverSocket->packetBag->GetLastNumber(Packet::ID_CHAT_RESPONSE);
					this->GetPeerName(socketAddress, socketPortNumber);
					socketAddress.Format("%s:%d : %s", (LPCTSTR)socketAddress, socketPortNumber, content.c_str());
					content = (LPCTSTR)socketAddress;
					if (packet != 0) {
						delete packet;
					}
					packet = new Packet(number + 1, Packet::ID_CHAT_RESPONSE, content);
				}
				//3.5.2. 채팅 내용을 보여주다.
				Viewer viewer(serverSocket->chatter->chattingForm);
				content += "\r\n";
				viewer.View(content);
			}
			//3.6. 종료 패킷이 아니면
			if (identifier != Packet::ID_SYCMY && identifier != Packet::ID_FINISH && identifier != Packet::ID_FINISHACK) {
				// 3.6.1. 모두에게 전달하다.
				serverSocket->SendDataAll(packet);
				//3.6.2. 패킷 가방에 패킷을 추가하다.
				serverSocket->packetBag->Add(packet);
			}
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