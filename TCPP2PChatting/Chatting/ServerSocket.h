#ifndef _SERVERSOCKET_H
#define _SERVERSOCKET_H
#include <afxsock.h>

class Chatter;
class ClientSocket;
class Packet;
class ServerSocket : public CAsyncSocket { //������� �񵿱������� �̷������ ����
public:
	Chatter *chatter;
	CPtrList clientSockets;

public:
	ServerSocket(Chatter *chatter = 0);
	virtual ~ServerSocket();

	//OnAccept - ������ ������ ��ٸ��� Ư�� ��Ʈ�� Ŭ���̾�Ʈ�� TCP ������ ���� �� �ڵ����� ȣ��ȴ�.
	virtual void OnAccept(int nErrorCode);

	void SendDataAll(Packet *packet);
	void SendDataAll(TCHAR(*buffer), size_t len);
	
	void CloseClientSocket(ClientSocket *clientSocket);
};

#endif //_SERVERSOCKET_H