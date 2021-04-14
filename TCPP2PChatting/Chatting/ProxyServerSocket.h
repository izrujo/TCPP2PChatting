#ifndef _PROXYSERVERSOCKET_H
#define _PROXYSERVERSOCKET_H
#include <afxsock.h>

class ServerSocket;
class ProxyServerSocket : public CAsyncSocket {
public:
	ServerSocket* serverSocket;
	CString ipAddress;
	UINT portNumber;

public:
	ProxyServerSocket(ServerSocket* serverSocket);
	virtual ~ProxyServerSocket();

	virtual int ReceiveFrom(void* lpBuf, int nBufLen, CString& rSocketAddress, UINT& rSocketPort, int nFlags);
};

#endif // _PROXYSERVERSOCKET_H