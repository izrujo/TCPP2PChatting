// ProxyServerSocket.cpp
/*
파일명칭 : 박컴학원 도메인으로 프록시 서버 소켓을 만든다.
*/
#include "ProxyServerSocket.h"
#include "ServerSocket.h"

ProxyServerSocket::ProxyServerSocket(ServerSocket* serverSocket) {
	this->serverSocket = serverSocket;
	this->ipAddress = "222.231.2.20";
	this->portNumber = 2180; // 2180 은 로그인을 할 때 사용하는 포트 번호
}

ProxyServerSocket::~ProxyServerSocket() {

}

int ProxyServerSocket::ReceiveFrom(void* lpBuf, int nBufLen, CString& rSocketAddress, UINT& rSocketPort, int nFlags) {



	return CAsyncSocket::ReceiveFrom(lpBuf, nBufLen, rSocketAddress, rSocketPort, nFlags);
}