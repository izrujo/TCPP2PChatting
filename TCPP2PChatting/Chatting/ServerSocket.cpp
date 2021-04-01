#include "ServerSocket.h"
#include "ClientSocket.h"
#include "Packet.h"
#include "PacketBag.h"
#include "Viewer.h"
#include "Chatter.h"

#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

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
	/*
	Three-Way HandShaking Flow

	1. 클라이언트 연결 요청
	=========================
	2. 서버 허락
	 2.1. 클라이언트에 연결
	 2.2. 동기화 성공 메시지 송신
	3, 클라이언트 동기화 성공 메시지 수신
	4. 새로운 클라이언트에게 피어 리스트 송신
	5. 접속 메시지 모두에게 송신
	=========================
	*/
	ClientSocket tempClient;
	ClientSocket* newClient;

	Packet* packet;

	CString packetMessage;
	CString socketAddress;
	CString ipAddress;
	UINT portNumber;

	LONG number;
	string ipInformation;

	BOOL onIsSucceed;

	// 1. 클라이언트 소켓을 받아들인다.
	onIsSucceed = Accept(tempClient);

	// 2. 클라이언트 소켓을 받아들이는데 성공하면
	if (onIsSucceed == TRUE) {
		// 2.1. IP 정보를 가져온다.
		tempClient.GetPeerName(ipAddress, portNumber);

		// 2.2. 새로운 클라이언트를 만든다.
		newClient = new ClientSocket;
		newClient->Create(portNumber, SOCK_STREAM, (LPCTSTR)ipAddress);

		// 2.3. 새로운 클라이언트에 연결한다.
		newClient->Connect((LPCTSTR)ipAddress, portNumber);

		// 2.4. 클라이언트에게 동기화 성공 메시지를 보낸다.
		number = this->packetBag->GetLastNumber(Packet::ID_SYCACK);// ID_IP => ID_SYC

		packetMessage.Format("%d:%d:%s:%d", 0, Packet::ID_SYCMY, (LPCTSTR)ipAddress, portNumber);
		packet = new Packet((LPCTSTR)packetMessage);

		newClient->SendData(packet);

		if (packet != 0) {
			delete packet;
		}

		packetMessage.Format("%d:%d:%s:%d", number + 1, Packet::ID_SYCACK, (LPCTSTR)ipAddress, portNumber);
		packet = new Packet((LPCTSTR)packetMessage);

		newClient->SendData(packet);

		if (packet != 0) {
			delete packet;
		}

		// 2.5. 패킷 가방에서 IP정보를 찾다.
		number = this->packetBag->FindIP(ipInformation);

		// 2.6. 못찾았으면
		if (number == 0) {
			// 2.6.1. 패킷을 만든다.
			number = this->packetBag->GetLastNumber(Packet::ID_SYC);// ID_IP => ID_SYC

			packetMessage.Format("%d:%d:%s:%d", number + 1, Packet::ID_SYC, (LPCTSTR)ipAddress, portNumber);
			packet = new Packet((LPCTSTR)packetMessage);

			// 2.6.3. 모두에게 전달한다.
			this->SendDataAll(packet);

			if (packet != 0) {
				delete packet;
			}

			// 2.6.4. 새로운 클라이언트에게 리스트를 송신한다.
			ClientSocket* client = NULL;
			POSITION previous = NULL;
			POSITION current = this->clientSockets.GetHeadPosition();
			while (previous != current && current != NULL) {
				client = (ClientSocket*)this->clientSockets.GetAt(current);
				client->GetPeerName(ipAddress, portNumber);

				socketAddress.Format("%s:%d", (LPCTSTR)ipAddress, portNumber);
				number = this->packetBag->FindIP((LPCTSTR)socketAddress);

				packet = new Packet(number, Packet::ID_SYC, (LPCTSTR)socketAddress);

				newClient->SendData(packet);
				if (packet != 0) {
					delete packet;
				}

				previous = current;
				this->clientSockets.GetNext(current);
			}

			// 2.6.4. 새로운 클라이언트를 리스트에 추가한다.
			this->clientSockets.AddTail(newClient);
			newClient->SetServerSocket(this);

			AfxMessageBox(_T("Other Peer Connected!!"));
		}
	}
	else {
		AfxMessageBox(_T("Error: Failed to accept new Client!"));
	}

#if 0
	//1. 클라이언트 소켓을 만들다.
	ClientSocket *newClient = new ClientSocket;
	//2. 받아들이다.
	BOOL onIsSucceed = Accept(*newClient);
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
			number = this->packetBag->GetLastNumber(Packet::ID_SYC);
			CString packetMessage;
			packetMessage.Format("%d:%d:%s:%d", number + 1, Packet::ID_SYC, (LPCTSTR)ipAddress, portNumber);
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
			//3.3.5. 누군가 채팅방에 접속하였음을 알리는 채팅 패킷을 만들다.
			CString comment;
			comment.Format("\r\n\r\n[%s]에서 채팅방에 접속하였습니다.\r\n\r\n", ipAddress);
			number = this->packetBag->GetLastNumber(Packet::ID_CHAT_RESPONSE);
			packet = new Packet(number + 1, Packet::ID_CHAT_RESPONSE, (LPCTSTR)comment);
			//3.3.6.모두에게 전달하다.
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
#endif
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
		client->SendData(packet);
		previous = current;
		this->clientSockets.GetNext(current);
	}
}

CString ServerSocket::GetInternalIpAddress() {
	CString insideIpAddress;

	IP_ADAPTER_INFO* pAdapterInfo = NULL;
	ULONG ulInBufLen; // ULong InBuffer

	GetAdaptersInfo(NULL, &ulInBufLen);

	pAdapterInfo = (IP_ADAPTER_INFO*)HeapAlloc(GetProcessHeap(), 0, ulInBufLen);

	GetAdaptersInfo(pAdapterInfo, &ulInBufLen);

	insideIpAddress = pAdapterInfo->IpAddressList.IpAddress.String;

	if (pAdapterInfo != 0) {
		HeapFree(GetProcessHeap(), 0, pAdapterInfo);
	}

	return insideIpAddress;
}

CString ServerSocket::GetExternalIpAddress() {// 외부 IP는 웹과 통신하여 알아내야 한다. mfc에 기능이 없는듯 하다.
	CString outsideIpAddress;

	return outsideIpAddress;
}

CString ServerSocket::GetDNSServerIpAddress() {
	CString outsideIpAddress;

	FIXED_INFO* pFixedInfo = NULL;
	ULONG ulOutBufLen;

	GetNetworkParams(NULL, &ulOutBufLen);

	pFixedInfo = (FIXED_INFO*)HeapAlloc(GetProcessHeap(), 0, ulOutBufLen);

	GetNetworkParams(pFixedInfo, &ulOutBufLen);

	outsideIpAddress = pFixedInfo->DnsServerList.IpAddress.String;

	if (pFixedInfo != 0) {
		HeapFree(GetProcessHeap(), 0, pFixedInfo);
	}

	return outsideIpAddress;
}