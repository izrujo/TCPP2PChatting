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
	//1. ���������� �����ϴ�.
	this->serverSocket = serverSocket;
}

void ClientSocket::OnClose(int nErrorCode) {
	//1. ������ �ݴ�.
	CSocket::OnClose(nErrorCode);
	//2. �������Ͽ��� �ݴ�.
	ServerSocket* serverSocket = (ServerSocket*)this->serverSocket;
	serverSocket->CloseClientSocket(this);
}

void ClientSocket::OnReceive(int nErrorCode) {
	ServerSocket* serverSocket = (ServerSocket*)this->serverSocket;

	CString socketAddress;
	UINT socketPortNumber;

	string ipAddress;
	Long portNumber;

	TCHAR buffer[1024];
	//1. ��Ŷ�� �޴�.
	if (this->Receive(buffer, sizeof(buffer)) > 0) {
		Packet* packet = new Packet(buffer);
		Packet::IDENTIFY identifier = packet->GetIdentifier();
		Long number = packet->GetNumber();
		string content = packet->GetContent();
		//2. ��Ŷ ���濡�� ã��.
		Long index = serverSocket->packetBag->Find(identifier, number);
		//3. ��ã������
		if (index == -1) {
			//3.1. ��ο��� �����ϴ�.
			serverSocket->SendDataAll(packet);
			//3.2. ip�����̸� ��ȭ�ڿ��� �����ϴ�.
			if (identifier == Packet::ID_IP) {
				//content�� ip�ּҿ� ��Ʈ��ȣ�� ������
				packet->GetIPInformations(&ipAddress, &portNumber);
				//CString strTemp = content.c_str();
				//Long punctutation = strTemp.Find(':');
				//ipAddress = strTemp.Left(index); // :�� ��ġ�� 0���̽����� index���� �߶󳻸� ��.
				//strTemp = strTemp.Mid(index + 1, strTemp.GetLength() - index - 1);
				//portNumber = _ttoi(strTemp);
				//content�� ip�ּҿ� ��Ʈ��ȣ�� ������
				serverSocket->chatter->Call(ipAddress, portNumber);
			}
			//3.3. ä���̸�
			else if (identifier == Packet::ID_CHAT_REQUEST || identifier == Packet::ID_CHAT_RESPONSE) {
				// 3.3.1. �Ƿ��� ä���̸� ���� �ּҸ� �߰��Ѵ�.
				if (identifier == Packet::ID_CHAT_REQUEST) {
					this->GetPeerName(socketAddress, socketPortNumber);

					socketAddress.Format("%s:%d : %s", (LPCTSTR)socketAddress, socketPortNumber, content.c_str());

					content = (LPCTSTR)socketAddress;
				}
				// 3.3.2. ä�� ������ �����ִ�.
				Viewer viewer(serverSocket->chatter->chattingForm);
				content+= "\r\n";
				viewer.View(content);
				
			}
			//3.4. ��Ŷ ���濡 ��Ŷ�� �߰��ϴ�.
			serverSocket->packetBag->Add(packet);
		}
	}

	CSocket::OnReceive(nErrorCode);
}

void ClientSocket::SendData(Packet* packet) {
	TCHAR(*buffer);
	LONG len;
	//1. �� Ŭ���̾�Ʈ ���ϰ� ����� ������ Ŭ���̾�Ʈ ���Ͽ� �����͸� ������.
	packet->GetPacketMessage(&buffer, &len);

	this->Send(buffer, len);

	if (buffer != 0) {
		delete[] buffer;
	}
}