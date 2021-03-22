#ifndef _CHATTINGFORM_H
#define _CHATTINGFORM_H

#include <afxwin.h>
#include <string>
using namespace std;

class TextEditingForm;
class ChatEditingForm;
class Chatter;
class UIGraph;

class ChattingForm : public CFrameWnd {
public:
	ChattingForm();
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);

public:
	TextEditingForm* chattingView;
	ChatEditingForm* chattingEdit;
	Chatter* chatter;
	UIGraph* sendButton;

	string ipAddress;
	Long portNumber;

protected:
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnCommandRange(UINT uID);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	
	DECLARE_MESSAGE_MAP()
};

#endif //_CHATTINGFORM_H