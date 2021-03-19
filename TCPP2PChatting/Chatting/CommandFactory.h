#ifndef _COMMANDFACTORY_H
#define _COMMANDFACTORY_H

#include "ChattingForm.h"

class Command;

class CommandFactory {
public:
	CommandFactory(ChattingForm* chattingForm = 0);
	CommandFactory(const CommandFactory& source);
	~CommandFactory();
	CommandFactory& operator=(const CommandFactory& source);

	Command* Make(UINT uID);

private:
	ChattingForm* chattingForm;
};

#endif //_COMMANDFACTORY_H