#include "CommandFactory.h"
#include "Commands.h"
#include "resource.h"

CommandFactory::CommandFactory(ChattingForm* chattingForm) {
	this->chattingForm = chattingForm;
}

CommandFactory::CommandFactory(const CommandFactory& source){
	this->chattingForm = source.chattingForm;
}

CommandFactory::~CommandFactory() {

}

CommandFactory& CommandFactory::operator=(const CommandFactory& source){
	this->chattingForm = source.chattingForm;

	return *this;
}

Command* CommandFactory::Make(UINT uID){
	Command* command = 0;
	
	if (uID == IDC_SEND_MESSAGE) {
		command = new SendCommand(this->chattingForm);
	}

	return command;
}