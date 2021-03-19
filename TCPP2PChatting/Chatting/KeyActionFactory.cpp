#include "KeyActionFactory.h"
#include "KeyActions.h"
#include "ChatEditingForm.h"
#include <WinUser.h>

KeyActionFactory::KeyActionFactory(ChatEditingForm* chatEditingForm) {
	this->chatEditingForm = chatEditingForm;
}

KeyActionFactory::KeyActionFactory(const KeyActionFactory& source) {
	this->chatEditingForm = source.chatEditingForm;
}

KeyActionFactory::~KeyActionFactory() {

}

KeyActionFactory& KeyActionFactory::operator=(const KeyActionFactory& source) {
	this->chatEditingForm = source.chatEditingForm;

	return *this;
}

KeyAction* KeyActionFactory::Make(UINT nChar) {
	KeyAction* keyAction = 0;

	SHORT isCtrl = GetKeyState(VK_CONTROL) & 0X8000;
	//SHORT isShift = GetKeyState(VK_SHIFT) & 0X8000;

	if (isCtrl && nChar == VK_RETURN) {
		keyAction = new CtrlEnterKeyAction(this->chatEditingForm);
	}

	return keyAction;
}