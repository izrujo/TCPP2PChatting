#ifndef _CHATTER_H
#define _CHATTER_H

#include "ServerSocket.h"
#include <string>

using namespace std;

class ChattingForm;
//class ServerSocket;

class Chatter {
public:
	Chatter(ChattingForm* chattingForm = 0);
	Chatter(const Chatter& source);
	~Chatter();
	Chatter& operator=(const Chatter& source);

	bool Call(string ipAddress, int portNumber);
	void Listen();
	void Speak();

public:
	ChattingForm* chattingForm;
	ServerSocket serverSocket;
};


#endif //_CHATTER_H