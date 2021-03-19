#ifndef _KEYACTIONFACTORY_H
#define _KEYACTIONFACTORY_H

#include <afxwin.h>

class KeyAction;
class ChatEditingForm;

class KeyActionFactory {
public:
	KeyActionFactory(ChatEditingForm* chatEditingForm = 0);
	KeyActionFactory(const KeyActionFactory& source);
	~KeyActionFactory();
	KeyActionFactory& operator=(const KeyActionFactory& source);

	KeyAction* Make(UINT nChar);
private:
	ChatEditingForm* chatEditingForm;
};

#endif // _KEYACTIONFACTORY_H