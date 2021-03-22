#include "Chatter.h"
#include "ChattingForm.h"
//#include "ServerSocket.h"
#include "ClientSocket.h"

Chatter::Chatter(ChattingForm* chattingForm)
	: serverSocket(this) {
	this->chattingForm = chattingForm;
	//this->serverSocket = new ServerSocket(this);
}

Chatter::Chatter(const Chatter& source) {
	this->chattingForm = source.chattingForm;
	//this->serverSocket = new ServerSocket(this);
}

Chatter::~Chatter() {

}

Chatter& Chatter::operator=(const Chatter& source) {
	this->chattingForm = source.chattingForm;

	//if (this->serverSocket != NULL) {
	//	delete this->serverSocket;
	//}
	//this->serverSocket = source.serverSocket;

	return *this;
}

bool Chatter::Call(string ipAddress, int portNumber) {
	ClientSocket* clientSocket = new ClientSocket;
	clientSocket->Create();
	bool isConnected = clientSocket->Connect(ipAddress.c_str(), portNumber);
	if (isConnected == true) {
		this->serverSocket.clientSockets.AddTail(clientSocket);
		clientSocket->SetServerSocket(&this->serverSocket);
	}
	else {
		clientSocket->Close();
		delete clientSocket;
	}

	return isConnected;
}

void Chatter::Listen() {
	if (!AfxSocketInit()) //���� ���̺귯�� �ʱ�ȭ
	{
		
	}
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