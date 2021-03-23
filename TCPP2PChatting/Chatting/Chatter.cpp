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
	clientSocket->Create();
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
	if (this->serverSocket.Create(2180, SOCK_STREAM)) //���� ����(���ε�Ǵ� ��Ʈ��ȣ, TCP ���� �÷���)
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
	//2. �����ִ�.
	Viewer viewer(this->chattingForm);
	viewer.View(speakMessage);
	//3. ä�� ��Ŷ�� �����.
	Long number = this->serverSocket.packetBag->GetLastNumber(Packet::ID_CHAT);
	CString packetMessage;
	packetMessage.Format("%d:%d:%s", number + 1, Packet::ID_CHAT, speakMessage);
	Packet* packet = new Packet((LPCTSTR)packetMessage);
	//4. ���� ���Ͽ��� ��ο��� �����͸� ������.
	this->serverSocket.SendDataAll(packet);
	//5. ��Ŷ�� ���濡 �߰��ϴ�.
	this->serverSocket.packetBag->Add(packet);
}