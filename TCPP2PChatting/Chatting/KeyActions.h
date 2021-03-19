#ifndef _KEYACTIONS_H
#define _KEYACTIONS_H

#include <afxwin.h>

class ChatEditingForm;

class KeyAction {
public:
	KeyAction(ChatEditingForm* chatEditingForm = 0);
	KeyAction(const KeyAction& source);
	virtual ~KeyAction() = 0;
	KeyAction& operator=(const KeyAction& source);

	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) = 0;
protected:
	ChatEditingForm* chatEditingForm;
};

class CtrlEnterKeyAction : public KeyAction {
public:
	CtrlEnterKeyAction(ChatEditingForm* chatEditingForm = 0);
	CtrlEnterKeyAction(const CtrlEnterKeyAction& source);
	virtual ~CtrlEnterKeyAction();
	CtrlEnterKeyAction& operator =(const CtrlEnterKeyAction& source);
	
	virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif //_KEYACTIONS_H