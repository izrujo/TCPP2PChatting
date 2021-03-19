#include "ChatEditingForm.h"
#include "ChattingForm.h"
#include "KeyActions.h"
#include "KeyActionFactory.h"

#include "resource.h"

#include "../TextEditor/Glyph.h"
#include "../TextEditor/Font.h"
#include "../TextEditor/resource.h"

BEGIN_MESSAGE_MAP(ChatEditingForm, TextEditingForm)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

ChatEditingForm::ChatEditingForm()
	: TextEditingForm() {
	this->isRequestForSend = FALSE;
}

int ChatEditingForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	TextEditingForm::OnCreate(lpCreateStruct);

	return 0;
}

void ChatEditingForm::OnClose() {
	TextEditingForm::OnClose();
}

void ChatEditingForm::OnPaint() {
	TextEditingForm::OnPaint();
}

void ChatEditingForm::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (nChar == VK_RETURN && this->note->GetLength() > 0 && this->current->GetLength() > 0 && 
		this->isRequestForSend == FALSE) {
		static_cast<ChattingForm*>(this->GetParent())->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_SEND_MESSAGE, 0));
	}
	if (nChar != VK_RETURN || (nChar == VK_RETURN && this->isRequestForSend == TRUE)) {
		TextEditingForm::OnChar(nChar, nRepCnt, nFlags);
	}
}

void ChatEditingForm::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	KeyActionFactory keyActionFactory(this);
	KeyAction* keyAction = keyActionFactory.Make(nChar);

	if (keyAction != 0) {
		keyAction->OnKeyDown(nChar, nRepCnt, nFlags);
		delete keyAction;
	}

	TextEditingForm::OnKeyDown(nChar, nRepCnt, nFlags);
}

void ChatEditingForm::OnHScroll(UINT nSBChat, UINT nPos, CScrollBar* pScrollBar) {
	TextEditingForm::OnHScroll(nSBChat, nPos, pScrollBar);
}

void ChatEditingForm::OnVScroll(UINT nSBChat, UINT nPos, CScrollBar* pScrollBar) {
	TextEditingForm::OnVScroll(nSBChat, nPos, pScrollBar);
}

BOOL ChatEditingForm::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {
	TextEditingForm::OnMouseWheel(nFlags, zDelta, pt);

	return TRUE;
}