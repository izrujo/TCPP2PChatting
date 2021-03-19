#include "Commands.h"
#include "ChattingForm.h"
#include "Viewer.h"
#include "ChatEditingForm.h"

#include "../TextEditor/TextEditingForm.h"
#include "../TextEditor/Glyph.h"
#include "../TextEditor/GlyphFactory.h"
#include "../TextEditor/ScrollController.h"

Command::Command(ChattingForm* chattingForm) {
	this->chattingForm = chattingForm;
}

Command::Command(const Command& source) {
	this->chattingForm = source.chattingForm;
}

Command::~Command() {

}

Command& Command::operator=(const Command& source) {
	this->chattingForm = source.chattingForm;

	return *this;
}

//SendCommand
SendCommand::SendCommand(ChattingForm* chattingForm)
	: Command(chattingForm) {

}

SendCommand::SendCommand(const SendCommand& source)
	: Command(source) {

}

SendCommand::~SendCommand() {

}

SendCommand& SendCommand::operator=(const SendCommand& source) {
	Command::operator=(source);

	return *this;
}

void SendCommand::Execute() {
	string content(": ");
	content += this->chattingForm->chattingEdit->note->GetContent();
	content += "\r\n";

	Viewer viewer(this->chattingForm);
	viewer.View(content);

	if (this->chattingForm->chattingEdit->note != NULL) {
		delete this->chattingForm->chattingEdit->note;
	}
	GlyphFactory glyphFactory;
	this->chattingForm->chattingEdit->note = glyphFactory.Make("");
	Glyph* line = glyphFactory.Make("\r\n");
	this->chattingForm->chattingEdit->note->Add(line);

	Long index = this->chattingForm->chattingEdit->note->First();
	this->chattingForm->chattingEdit->current = this->chattingForm->chattingEdit->note->GetAt(index);
	this->chattingForm->chattingEdit->current->First();

	if (this->chattingForm->chattingEdit->scrollController != NULL) {
		delete this->chattingForm->chattingEdit->scrollController;
		this->chattingForm->chattingEdit->scrollController = new ScrollController(this->chattingForm->chattingEdit);
	}
	this->chattingForm->chattingEdit->Notify();
}

Command* SendCommand::Clone() {
	return new SendCommand(*this);
}