#include "KeyActions.h"
#include "ChattingForm.h"
#include "ChatEditingForm.h"
#include "resource.h"

//KeyAction
KeyAction::KeyAction(ChatEditingForm* chatEditingForm) {
	this->chatEditingForm = chatEditingForm;
}

KeyAction::KeyAction(const KeyAction& source) {
	this->chatEditingForm = source.chatEditingForm;
}

KeyAction::~KeyAction() {

}

KeyAction& KeyAction::operator=(const KeyAction& source) {
	this->chatEditingForm = source.chatEditingForm;

	return *this;
}

//CtrlEnterKeyAction
CtrlEnterKeyAction::CtrlEnterKeyAction(ChatEditingForm* chatEditingForm)
	: KeyAction(chatEditingForm) {
}

CtrlEnterKeyAction::CtrlEnterKeyAction(const CtrlEnterKeyAction& source)
	: KeyAction(source) {
}

CtrlEnterKeyAction::~CtrlEnterKeyAction() {
}

void CtrlEnterKeyAction::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	this->chatEditingForm->isRequestForSend = TRUE;
	static_cast<TextEditingForm*>(this->chatEditingForm)->SendMessage(WM_CHAR, MAKEWPARAM(VK_RETURN, 0));
	this->chatEditingForm->isRequestForSend = FALSE;
}

CtrlEnterKeyAction& CtrlEnterKeyAction::operator =(const CtrlEnterKeyAction& source) {
	KeyAction::operator=(source);

	return *this;
}