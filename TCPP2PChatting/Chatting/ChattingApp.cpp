#include "ChattingApp.h"
#include "ChattingForm.h"

BOOL ChattingApp::InitInstance() {
	ChattingForm* chattingForm = new ChattingForm;
	chattingForm->Create(NULL, "CNChatting");
	chattingForm->ShowWindow(SW_MAXIMIZE);
	chattingForm->UpdateWindow();
	this->m_pMainWnd = chattingForm;

	return TRUE;
}
ChattingApp chattingApp;