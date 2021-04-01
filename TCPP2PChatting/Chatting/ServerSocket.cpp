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

	1. Ŭ���̾�Ʈ ���� ��û
	=========================
	2. ���� ���
	 2.1. Ŭ���̾�Ʈ�� ����
	 2.2. ����ȭ ���� �޽��� �۽�
	3, Ŭ���̾�Ʈ ����ȭ ���� �޽��� ����
	4. ���ο� Ŭ���̾�Ʈ���� �Ǿ� ����Ʈ �۽�
	5. ���� �޽��� ��ο��� �۽�
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

	// 1. Ŭ���̾�Ʈ ������ �޾Ƶ��δ�.
	onIsSucceed = Accept(tempClient);

	// 2. Ŭ���̾�Ʈ ������ �޾Ƶ��̴µ� �����ϸ�
	if (onIsSucceed == TRUE) {
		// 2.1. IP ������ �����´�.
		tempClient.GetPeerName(ipAddress, portNumber);

		// 2.2. ���ο� Ŭ���̾�Ʈ�� �����.
		newClient = new ClientSocket;
		newClient->Create(portNumber, SOCK_STREAM, (LPCTSTR)ipAddress);

		// 2.3. ���ο� Ŭ���̾�Ʈ�� �����Ѵ�.
		newClient->Connect((LPCTSTR)ipAddress, portNumber);

		// 2.4. Ŭ���̾�Ʈ���� ����ȭ ���� �޽����� ������.
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

		// 2.5. ��Ŷ ���濡�� IP������ ã��.
		number = this->packetBag->FindIP(ipInformation);

		// 2.6. ��ã������
		if (number == 0) {
			// 2.6.1. ��Ŷ�� �����.
			number = this->packetBag->GetLastNumber(Packet::ID_SYC);// ID_IP => ID_SYC

			packetMessage.Format("%d:%d:%s:%d", number + 1, Packet::ID_SYC, (LPCTSTR)ipAddress, portNumber);
			packet = new Packet((LPCTSTR)packetMessage);

			// 2.6.3. ��ο��� �����Ѵ�.
			this->SendDataAll(packet);

			if (packet != 0) {
				delete packet;
			}

			// 2.6.4. ���ο� Ŭ���̾�Ʈ���� ����Ʈ�� �۽��Ѵ�.
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

			// 2.6.4. ���ο� Ŭ���̾�Ʈ�� ����Ʈ�� �߰��Ѵ�.
			this->clientSockets.AddTail(newClient);
			newClient->SetServerSocket(this);

			AfxMessageBox(_T("Other Peer Connected!!"));
		}
	}
	else {
		AfxMessageBox(_T("Error: Failed to accept new Client!"));
	}

#if 0
	//1. Ŭ���̾�Ʈ ������ �����.
	ClientSocket *newClient = new ClientSocket;
	//2. �޾Ƶ��̴�.
	BOOL onIsSucceed = Accept(*newClient);
	//3. �޾Ƶ��̴µ� ����������
	if (onIsSucceed == TRUE) {
		//3.1. ip������ ��������.
		CString ipAddress;
		UINT portNumber;
		newClient->GetPeerName(ipAddress, portNumber);
		//3.2. ��Ŷ ���濡�� IP������ ã��.
		string ipInformation;
		Long number = this->packetBag->FindIP(ipInformation);
		//3.3. ��ã������
		if (number == 0) {
			//3.3.1. ��Ŷ�� �����.
			number = this->packetBag->GetLastNumber(Packet::ID_SYC);
			CString packetMessage;
			packetMessage.Format("%d:%d:%s:%d", number + 1, Packet::ID_SYC, (LPCTSTR)ipAddress, portNumber);
			Packet* packet = new Packet((LPCTSTR)packetMessage);
			//3.3.2. ��ο��� �����ϴ�.
			this->SendDataAll(packet);
			//3.3.3. ����Ʈ�� �߰��ϴ�.
			this->clientSockets.AddTail(newClient);
			//3.3.4. Ŭ���̾�Ʈ ������ ���� ������ �����ϴ�.
			newClient->SetServerSocket(this);
			if (packet != 0) {
				delete packet;
			}
			//3.3.5. ������ ä�ù濡 �����Ͽ����� �˸��� ä�� ��Ŷ�� �����.
			CString comment;
			comment.Format("\r\n\r\n[%s]���� ä�ù濡 �����Ͽ����ϴ�.\r\n\r\n", ipAddress);
			number = this->packetBag->GetLastNumber(Packet::ID_CHAT_RESPONSE);
			packet = new Packet(number + 1, Packet::ID_CHAT_RESPONSE, (LPCTSTR)comment);
			//3.3.6.��ο��� �����ϴ�.
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
	//4. ���������� Ŭ���̾�Ʈ ������ ���ִ�.
	else {
		delete newClient;
		AfxMessageBox(_T("Error: Failed to accept new Client!"));
	}
#endif
	CAsyncSocket::OnAccept(nErrorCode);
}

void ServerSocket::CloseClientSocket(ClientSocket* clientSocket) {
	POSITION position;
	//1. ����Ʈ���� Ŭ���̾�Ʈ ������ ã��.
	position = this->clientSockets.Find(clientSocket);
	//2. ã������
	if (position != NULL) {
		if (clientSocket != NULL) {
			//2.1. Ŭ���̾�Ʈ ������ ����� �����ϴ�.
			clientSocket->ShutDown(); //��� ����
			//2.2. Ŭ���̾�Ʈ ������ �ݴ�.
			clientSocket->Close(); //���� �ݱ�
		}
		//2.3. Ŭ���̾�Ʈ ������ ����Ʈ���� �����.
		this->clientSockets.RemoveAt(position); //����Ʈ���� ����
		//2.4. Ŭ���̾�Ʈ ������ ���ִ�.
		delete clientSocket; //����
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

CString ServerSocket::GetExternalIpAddress() {// �ܺ� IP�� ���� ����Ͽ� �˾Ƴ��� �Ѵ�. mfc�� ����� ���µ� �ϴ�.
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