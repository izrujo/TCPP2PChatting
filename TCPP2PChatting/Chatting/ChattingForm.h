#ifndef _CHATTINGFORM_H
#define _CHATTINGFORM_H

#include <afxwin.h>

class TextEditingForm;
class Chatter;

class ChattingForm : public CFrameWnd {
public:
	ChattingForm();
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);

public:
	TextEditingForm* chattingView;
	TextEditingForm* chattingEdit;
	Chatter* chatter;

protected:
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnCommandRange(UINT uID);
	
	DECLARE_MESSAGE_MAP()
};

#endif //_CHATTINGFORM_H