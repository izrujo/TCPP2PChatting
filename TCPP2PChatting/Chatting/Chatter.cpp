#include "Chatter.h"
#include "ChattingForm.h"
#include "ClientSocket.h"
#include "ChatEditingForm.h"
#include "Viewer.h"
#include "Packet.h"
#include "PacketBag.h"
#include "../TextEditor/Glyph.h"

Chatter::Chatter(ChattingForm* chattingForm)
	: serverSocket(this) {
	this->chattingForm = chattingForm;

	this->onIsFinished = FALSE;

	if (!AfxSocketInit()) //���� ���̺귯�� �ʱ�ȭ
	{
		AfxMessageBox(_T("ERROR: ���� �ʱ�ȭ ����"));
	}
}

Chatter::Chatter(const Chatter& source)
	: serverSocket(source.serverSocket) {
	this->chattingForm = source.chattingForm;
}

Chatter::~Chatter() {

}

Chatter& Chatter::operator=(const Chatter& source) {
	this->chattingForm = source.chattingForm;
	this->serverSocket = source.serverSocket;

	return *this;
}

bool Chatter::Call(string ipAddress, int portNumber) {
	//1. Ŭ���̾�Ʈ ������ �����ϴ�.
	ClientSocket* clientSocket = new ClientSocket;
	clientSocket->Create(portNumber, SOCK_STREAM, ipAddress.c_str());
	//2. Ŭ���̾�Ʈ ������ �����ϴ�.
	bool isConnected = clientSocket->Connect(ipAddress.c_str(), portNumber);
	//3. ���ῡ ����������
	if (isConnected == true) {
		//3.1. ���� ������ ����Ʈ�� �߰��ϴ�.
		this->serverSocket.clientSockets.AddTail(clientSocket);
		//3.2. Ŭ���̾�Ʈ ������ ���� ������ �����ϴ�.
		clientSocket->SetServerSocket(&this->serverSocket);
	}
	//4. ���ῡ ����������
	else {
		//4.1. Ŭ���̾�Ʈ ������ �ݴ�.
		clientSocket->Close();
		//4.2. Ŭ���̾�Ʈ ������ ���ִ�.
		delete clientSocket;
	}

	return isConnected;
}

void Chatter::Listen() {
	//��Ʈ��ȣ : ��Ʈ������ ��Ģ ���� �ÿ� ������ ���� ��Ʈ ��ȣ
	BOOL onIsSucceed = this->serverSocket.Create();
	if (onIsSucceed == TRUE) //���� ����(���ε�Ǵ� ��Ʈ��ȣ, TCP ���� �÷���)
	{
		if (!this->serverSocket.Listen()) //������ Ŭ���̾�Ʈ�� ������ ���� �� �ִ� ���·� ����
		{
			AfxMessageBox(_T("ERROR: Listen() return FALSE"));
		}
	}
	else
	{
		DWORD error = GetLastError();
		AfxMessageBox(_T("ERROR: Failed to create server socket!"));
	}
}

void Chatter::Speak() {
	//1. ���� ������ ��������.
	string speakMessage = this->chattingForm->chattingEdit->note->GetContent();
	//2. ä�� ��Ŷ�� �����.
	CString packetMessage;
	packetMessage.Format("%d:%d:%s", 0, Packet::ID_CHAT_REQUEST, speakMessage.c_str());
	Packet* packet = new Packet((LPCTSTR)packetMessage);
	//3. ���� ���Ͽ��� ��ο��� �����͸� ������.
	this->serverSocket.SendDataAll(packet);

	if (packet != 0) {
		delete packet;
	}
}