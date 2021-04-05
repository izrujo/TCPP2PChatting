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

	// 1. Ŭ���̾�Ʈ ������ �޾Ƶ��δ�.
	onIsSucceed = Accept(tempClient);// ����

	// 2. Ŭ���̾�Ʈ ������ �޾Ƶ��̴µ� �����ϸ�
	if (onIsSucceed == TRUE) {
		// 2.1. IP ������ �����´�.
		tempClient.GetPeerName(ipAddress, portNumber);

		// 2.2. �̹� ����� Ŭ���̾�Ʈ���� ã�ƺ���.
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

		// 2.3. �̹� ����� Ŭ���̾�Ʈ�� �ƴϸ�
		if (onIsFinding != TRUE) {
			// 2.3.1. ���ο� Ŭ���̾�Ʈ�� �����.
			newClient = new ClientSocket;
			newClient->Create(portNumber, SOCK_STREAM, (LPCTSTR)ipAddress);

			// 2.3.2. ���ο� Ŭ���̾�Ʈ�� �����Ѵ�.
			newClient->Connect((LPCTSTR)ipAddress, portNumber);

			// 2.3.3. Ŭ���̾�Ʈ���� �ڽ��� �ּҸ� �˷��ش�.
			number = this->packetBag->GetLastNumber(Packet::ID_SYCACK);// ID_IP => ID_SYC

			packetMessage.Format("%d:%d:%s:%d", 0, Packet::ID_SYCMY, (LPCTSTR)ipAddress, portNumber);
			packet = new Packet((LPCTSTR)packetMessage);

			newClient->SendData(packet);

			if (packet != 0) {
				delete packet;
			}

			// 2.3.4. Ŭ���̾�Ʈ���� ����ȭ ���� �޽����� ������.
			packetMessage.Format("%d:%d:%s:%d", number + 1, Packet::ID_SYCACK, (LPCTSTR)ipAddress, portNumber);
			packet = new Packet((LPCTSTR)packetMessage);

			newClient->SendData(packet);

			if (packet != 0) {
				delete packet;
			}

			// 2.3.5. ���ο� Ŭ���̾�Ʈ���� Ŭ���̾�Ʈ ����Ʈ�� �۽��Ѵ�.
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

			// 2.3.6. ���ο� Ŭ���̾�Ʈ�� ����Ʈ�� �߰��Ѵ�.
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
	// 1. ������ ���� ����Ʈ ��ġ���� �����.
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

	// 2. ��ο��� ������. + �����ϸ� ��ġ���� ����Ѵ�.
	current = this->clientSockets.GetHeadPosition();
	while (previous != current && current != NULL) {
		client = (ClientSocket*)this->clientSockets.GetAt(current);
		retValue = client->SendData(packet);

		// 2.1. �����ϸ� ��ġ�� �����Ѵ�.
		if (retValue <= 0) {
			failSocketList.AddTail(client);
		}

		previous = current;
		this->clientSockets.GetNext(current);
	}

	// 3. ��� ������ ������ 3ȸ �ݺ��Ѵ�.
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

	// 4. Ư�� �Ǿ ������ ���������� ������ ����Ʈ��ŭ �ݺ��Ѵ�.
	i = 0;
	failPrevious = NULL;
	failCurrent = failSocketList.GetHeadPosition();
	while (i < failSocketCount && failSocketCount > 0 && failSocketCount < this->clientSockets.GetCount()) {
		failPrevious = failCurrent;
		failSocketList.GetNext(failPrevious);

		// 4.1. Ŭ���̾�Ʈ ip ������ �д´�.
		failClient = (ClientSocket*)failSocketList.GetAt(failCurrent);

		failClient->GetPeerName(failIPAddress, failPortNumber);
		failSocketAddress.Format("%s:%d", (LPCTSTR)failIPAddress, failPortNumber);

		// 4.2. ��Ŷ ���濡�� ���� �ּҷ� ã�´�.
		packetIndex = this->packetBag->Find((LPCTSTR)failSocketAddress);

		// 4.3. ��Ŷ ���濡�� �����.
		if (packetIndex > -1) {
			this->packetBag->Remove(packetIndex);
		}

		// 4.4. Ŭ���̾�Ʈ ����Ʈ���� �����.
		this->CloseClientSocket(failClient);

		failCurrent = failPrevious;

		i++;
	}

	// 5. ��� �Ǿ�� ������ ���������� 3ȸ �ӽ� ������ �õ��Ѵ�. => ���߿��� ����� ������
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