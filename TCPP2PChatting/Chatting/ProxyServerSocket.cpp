// ProxyServerSocket.cpp
/*
���ϸ�Ī : �����п� ���������� ���Ͻ� ���� ������ �����.
*/
#include "ProxyServerSocket.h"
#include "ServerSocket.h"

ProxyServerSocket::ProxyServerSocket(ServerSocket* serverSocket) {
	this->serverSocket = serverSocket;
	this->ipAddress = "222.231.2.20";
	this->portNumber = 2180; // 2180 �� �α����� �� �� ����ϴ� ��Ʈ ��ȣ
}

ProxyServerSocket::~ProxyServerSocket() {

}

int ProxyServerSocket::ReceiveFrom(void* lpBuf, int nBufLen, CString& rSocketAddress, UINT& rSocketPort, int nFlags) {



	return CAsyncSocket::ReceiveFrom(lpBuf, nBufLen, rSocketAddress, rSocketPort, nFlags);
}