#ifndef _CHATEDITINGFORM_H
#define _CHATEDITINGFORM_H

#include "../TextEditor/TextEditingForm.h"

class ChatEditingForm : public TextEditingForm {
public:
	ChatEditingForm();
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);

public:
	BOOL isRequestForSend;
protected:
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	DECLARE_MESSAGE_MAP()

};

#endif //_CHATEDITINGFORM_H