#ifndef _CLIENTSOCKET_H
#define _CLIENTSOCKET_H

#include <afxsock.h>

class ServerSocket;
class Packet;

class ClientSocket : public CSocket {
public:
	ClientSocket();
	ClientSocket(const ClientSocket& source);
	virtual ~ClientSocket();
	ClientSocket& operator=(const ClientSocket& source);

	void SetServerSocket(ServerSocket* serverSocket);
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	void SendData(Packet* packet);

public:
	ServerSocket* serverSocket;
};

#endif //_CLIENTSOCKET_H