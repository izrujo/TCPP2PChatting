#ifndef _COMMANDS_H
#define _COMMANDS_H

class ChattingForm;

class Command {
public:
	Command(ChattingForm* chattingForm = 0);
	Command(const Command& source);
	virtual ~Command() = 0;
	Command& operator=(const Command& source);

	virtual void Execute() = 0;
	virtual Command* Clone() = 0;

protected:
	ChattingForm* chattingForm;
};

class SendCommand : public Command {
public:
	SendCommand(ChattingForm* chattingForm = 0);
	SendCommand(const SendCommand& source);
	virtual ~SendCommand();
	SendCommand& operator=(const SendCommand& source);

	virtual void Execute();
	virtual Command* Clone();
};

#endif //_COMMANDS_H