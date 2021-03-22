#ifndef _CLIENTSOCKET_H
#define _CLIENTSOCKET_H

#include <afxsock.h>

class ServerSocket;
class Packet;
class ClientSocket : public CSocket { //������� ���������� �̷������ ����
public:
	ServerSocket* serverSocket;

public:
	ClientSocket();
	virtual ~ClientSocket();

	void SetServerSocket(ServerSocket* serverSocket);
	void SendData(Packet *packet);
	//ä�� �޽����� �޾��� �� ȣ��ȴ�. �޽����� �����ϰ� ����� ��� Ŭ���̾�Ʈ���� �����Ѵ�.
	virtual void OnReceive(int nErrorCode);
	//������ ����Ǵ� ������ ȣ��ȴ�.
	virtual void OnClose(int nErrorCode);
};

#endif //_CLIENTSOCKET_H