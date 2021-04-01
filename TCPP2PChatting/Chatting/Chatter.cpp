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

	if (!AfxSocketInit()) //소켓 라이브러리 초기화
	{
		AfxMessageBox(_T("ERROR: 소켓 초기화 실패"));
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
	//1. 클라이언트 소켓을 생성하다.
	ClientSocket* clientSocket = new ClientSocket;
	clientSocket->Create(portNumber, SOCK_STREAM, ipAddress.c_str());
	//2. 클라이언트 소켓을 연결하다.
	bool isConnected = clientSocket->Connect(ipAddress.c_str(), portNumber);
	//3. 연결에 성공했으면
	if (isConnected == true) {
		//3.1. 서버 소켓의 리스트에 추가하다.
		this->serverSocket.clientSockets.AddTail(clientSocket);
		//3.2. 클라이언트 소켓의 서버 소켓을 설정하다.
		clientSocket->SetServerSocket(&this->serverSocket);
	}
	//4. 연결에 실패했으면
	else {
		//4.1. 클라이언트 소켓을 닫다.
		clientSocket->Close();
		//4.2. 클라이언트 소켓을 없애다.
		delete clientSocket;
	}

	return isConnected;
}

void Chatter::Listen() {
	//포트번호 : 포트포워딩 규칙 설정 시에 설정한 내부 포트 번호
	BOOL onIsSucceed = this->serverSocket.Create();
	if (onIsSucceed == TRUE) //소켓 생성(바인드되는 포트번호, TCP 소켓 플래그)
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

void Chatter::Speak() {
	//1. 말할 내용을 가져오다.
	string speakMessage = this->chattingForm->chattingEdit->note->GetContent();
	//2. 채팅 패킷을 만들다.
	CString packetMessage;
	packetMessage.Format("%d:%d:%s", 0, Packet::ID_CHAT_REQUEST, speakMessage.c_str());
	Packet* packet = new Packet((LPCTSTR)packetMessage);
	//3. 서버 소켓에서 모두에게 데이터를 보내다.
	this->serverSocket.SendDataAll(packet);

	if (packet != 0) {
		delete packet;
	}
}