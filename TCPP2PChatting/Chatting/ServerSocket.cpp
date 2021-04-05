#include "ServerSocket.h"
#include "ClientSocket.h"
#include "Packet.h"
#include "PacketBag.h"
#include "Viewer.h"
#include "ChattingForm.h"
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
	ClientSocket* client = NULL;

	POSITION previous = NULL;
	POSITION current;

	Packet* packet;

	CString packetMessage;
	CString socketAddress;
	CString ipAddress;
	CString otherIPAddress;
	UINT portNumber;
	UINT otherPortNumber;

	LONG number;
	string ipInformation;

	BOOL onIsSucceed;
	BOOL onIsFinding = FALSE;

	// 1. 클라이언트 소켓을 받아들인다.
	onIsSucceed = Accept(tempClient);// ㅇㅇ

	// 2. 클라이언트 소켓을 받아들이는데 성공하면
	if (onIsSucceed == TRUE) {
		// 2.1. IP 정보를 가져온다.
		tempClient.GetPeerName(ipAddress, portNumber);

		// 2.2. 이미 연결된 클라이언트인지 찾아본다.
		current = this->clientSockets.GetHeadPosition();
		while (previous != current && current != NULL && onIsFinding != TRUE) {
			client = (ClientSocket*)this->clientSockets.GetAt(current);
			
			client->GetPeerName(otherIPAddress, otherPortNumber);
			if (ipAddress == otherIPAddress && portNumber == otherPortNumber) {
				onIsFinding = TRUE;
			}

			previous = current;
			this->clientSockets.GetNext(current);
		}

		// 2.3. 이미 연결된 클라이언트가 아니면
		if (onIsFinding != TRUE) {
			// 2.3.1. 새로운 클라이언트를 만든다.
			newClient = new ClientSocket;
			newClient->Create(portNumber, SOCK_STREAM, (LPCTSTR)ipAddress);

			// 2.3.2. 새로운 클라이언트에 연결한다.
			newClient->Connect((LPCTSTR)ipAddress, portNumber);

			// 2.3.3. 클라이언트에게 자신의 주소를 알려준다.
			number = this->packetBag->GetLastNumber(Packet::ID_SYCACK);// ID_IP => ID_SYC

			packetMessage.Format("%d:%d:%s:%d", 0, Packet::ID_SYCMY, (LPCTSTR)ipAddress, portNumber);
			packet = new Packet((LPCTSTR)packetMessage);

			newClient->SendData(packet);

			if (packet != 0) {
				delete packet;
			}

			// 2.3.4. 클라이언트에게 동기화 성공 메시지를 보낸다.
			packetMessage.Format("%d:%d:%s:%d", number + 1, Packet::ID_SYCACK, (LPCTSTR)ipAddress, portNumber);
			packet = new Packet((LPCTSTR)packetMessage);

			newClient->SendData(packet);

			if (packet != 0) {
				delete packet;
			}

			// 2.3.5. 새로운 클라이언트에게 클라이언트 리스트를 송신한다.
			ClientSocket* client = NULL;
			POSITION previous = NULL;
			POSITION current = this->clientSockets.GetHeadPosition();
			while (previous != current && current != NULL) {
				client = (ClientSocket*)this->clientSockets.GetAt(current);
				client->GetPeerName(ipAddress, portNumber);

				socketAddress.Format("%s:%d", (LPCTSTR)ipAddress, portNumber);
				number = this->packetBag->FindNumber((LPCTSTR)socketAddress);

				packet = new Packet(number, Packet::ID_SYC, (LPCTSTR)socketAddress);

				newClient->SendData(packet);
				if (packet != 0) {
					delete packet;
				}

				previous = current;
				this->clientSockets.GetNext(current);
			}

			// 2.3.6. 새로운 클라이언트를 리스트에 추가한다.
			this->clientSockets.AddTail(newClient);
			newClient->SetServerSocket(this);

			AfxMessageBox(_T("Other Peer Connected!!"));
		}
	}
	else {
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
	// 1. 실패한 소켓 리스트 위치들을 만든다.
	ClientSocket* failClient = NULL;
	ClientSocket* client = NULL;

	CPtrList failSocketList;
	POSITION failPrevious;
	POSITION failCurrent;
	POSITION previous = NULL;
	POSITION current;

	CString failSocketAddress;
	CString failIPAddress;
	UINT failPortNumber;

	INT retValue;

	LONG failSocketCount = 0;
	LONG packetIndex;
	LONG i;

	bool onIsConnected = false;

	// 2. 모두에게 보낸다. + 실패하면 위치들을 기록한다.
	current = this->clientSockets.GetHeadPosition();
	while (previous != current && current != NULL) {
		client = (ClientSocket*)this->clientSockets.GetAt(current);
		retValue = client->SendData(packet);

		// 2.1. 실패하면 위치를 저장한다.
		if (retValue <= 0) {
			failSocketList.AddTail(client);
		}

		previous = current;
		this->clientSockets.GetNext(current);
	}

	// 3. 모두 성공할 때까지 3회 반복한다.
	failSocketCount = failSocketList.GetCount();
	i = 1;
	while (i <= 3 && failSocketCount > 0) {
		failPrevious = NULL;
		failCurrent = failSocketList.GetHeadPosition();
		while (failPrevious != failCurrent && failCurrent != NULL) {
			failClient = (ClientSocket*)failSocketList.GetAt(failCurrent);
			retValue = failClient->SendData(packet);

			if (retValue < 0 && failCurrent != failSocketList.GetTailPosition()) {
				failPrevious = failCurrent;
				failSocketList.GetNext(failPrevious);
				failSocketList.RemoveAt(failCurrent);
				failCurrent = failPrevious;

				failSocketCount--;
			}
			else if(retValue < 0) {
				failSocketList.RemoveAt(failCurrent);
				failCurrent = failSocketList.GetTailPosition();

				failSocketCount--;
			}

			failPrevious = failCurrent;
			failSocketList.GetNext(failCurrent);
		}
		i++;
	}

	// 4. 특정 피어가 연결이 끊어졌으면 실패한 리스트만큼 반복한다.
	i = 0;
	failPrevious = NULL;
	failCurrent = failSocketList.GetHeadPosition();
	while (i < failSocketCount && failSocketCount > 0 && failSocketCount < this->clientSockets.GetCount()) {
		failPrevious = failCurrent;
		failSocketList.GetNext(failPrevious);

		// 4.1. 클라이언트 ip 정보를 읽는다.
		failClient = (ClientSocket*)failSocketList.GetAt(failCurrent);

		failClient->GetPeerName(failIPAddress, failPortNumber);
		failSocketAddress.Format("%s:%d", (LPCTSTR)failIPAddress, failPortNumber);

		// 4.2. 패킷 가방에서 소켓 주소로 찾는다.
		packetIndex = this->packetBag->Find((LPCTSTR)failSocketAddress);

		// 4.3. 패킷 가방에서 지운다.
		if (packetIndex > -1) {
			this->packetBag->Remove(packetIndex);
		}

		// 4.4. 클라이언트 리스트에서 지운다.
		this->CloseClientSocket(failClient);

		failCurrent = failPrevious;

		i++;
	}

	// 5. 모든 피어와 연결이 끊어졌으면 3회 임시 연결을 시도한다. => 나중에는 연결될 때까지
	i = 1;
	while (i <= 3 && failSocketCount > 0 && failSocketCount >= this->clientSockets.GetCount() && onIsConnected != false) {
		onIsConnected = this->chatter->Call(this->chatter->chattingForm->ipAddress, this->chatter->chattingForm->portNumber);
		i++;
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