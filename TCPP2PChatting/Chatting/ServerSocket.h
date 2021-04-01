#ifndef _SERVERSOCKET_H
#define _SERVERSOCKET_H

#include <afxsock.h>

class Chatter;
class PacketBag;
class ClientSocket;
class Packet;

class ServerSocket : public CAsyncSocket {
public:
	ServerSocket(Chatter *chatter = 0);
	ServerSocket(const ServerSocket& source);
	virtual ~ServerSocket();
	ServerSocket& operator=(const ServerSocket& source);

	virtual void OnAccept(int nErrorCode);
	void CloseClientSocket(ClientSocket *clientSocket);
	void SendDataAll(Packet *packet);

	CString GetInternalIpAddress();
	CString GetExternalIpAddress();// »ç¿ë X
	CString GetDNSServerIpAddress();

public:
	CPtrList clientSockets;
	Chatter* chatter;
	PacketBag* packetBag;
};

#endif //_SERVERSOCKET_H