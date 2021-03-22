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
	if (!AfxSocketInit()) //소켓 라이브러리 초기화
	{
		
	}
	//포트번호 : 포트포워딩 규칙 설정 시에 설정한 내부 포트 번호
	if (this->serverSocket.Create(2180, SOCK_STREAM)) //소켓 생성(바인드되는 포트번호, TCP 소켓 플래그)
	{
		if (!this->serverSocket.Listen()) //서버가 클라이언트의 접속을 받을 수 있는 상태로 설정
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