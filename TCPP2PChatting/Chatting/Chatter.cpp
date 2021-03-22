#include "Chatter.h"
#include "ChattingForm.h"
#include "ServerSocket.h"
#include "ClientSocket.h"

Chatter::Chatter(ChattingForm* chattingForm) {
	this->chattingForm = chattingForm;
	this->serverSocket = new ServerSocket(this);
}

Chatter::Chatter(const Chatter& source) {
	this->chattingForm = source.chattingForm;
	this->serverSocket = new ServerSocket(this);
}

Chatter::~Chatter() {

}

Chatter& Chatter::operator=(const Chatter& source) {
	this->chattingForm = source.chattingForm;

	if (this->serverSocket != NULL) {
		delete this->serverSocket;
	}
	this->serverSocket = source.serverSocket;

	return *this;
}

bool Chatter::Call(string ipAddress, int portNumber) {
	ClientSocket* clientSocket = new ClientSocket;
	clientSocket->Create();
	bool isConnected = clientSocket->Connect(ipAddress.c_str(), portNumber);
	if (isConnected == true) {
		this->serverSocket->clientSockets.AddTail(clientSocket);
		clientSocket->SetServerSocket(this->serverSocket);
	}
	else {
		clientSocket->Close();
		delete clientSocket;
	}

	return isConnected;
}