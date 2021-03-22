#ifndef _SERVERSOCKET_H
#define _SERVERSOCKET_H
#include <afxsock.h>

class Chatter;
class ClientSocket;
class Packet;
class ServerSocket : public CAsyncSocket { //입출력이 비동기적으로 이루어지는 소켓
public:
	Chatter *chatter;
	CPtrList clientSockets;

public:
	ServerSocket(Chatter *chatter = 0);
	virtual ~ServerSocket();

	//OnAccept - 서버가 접속을 기다리는 특정 포트로 클라이언트의 TCP 접속이 있을 때 자동으로 호출된다.
	virtual void OnAccept(int nErrorCode);

	void SendDataAll(Packet *packet);
	void SendDataAll(TCHAR(*buffer), size_t len);
	
	void CloseClientSocket(ClientSocket *clientSocket);
};

#endif //_SERVERSOCKET_H