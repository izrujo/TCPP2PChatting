#include "ChattingForm.h"

#include "../TextEditor/TextEditingForm.h"

#define MARGIN 10

BEGIN_MESSAGE_MAP(ChattingForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_COMMAND_RANGE(0, 0, OnCommandRange)
END_MESSAGE_MAP()

ChattingForm::ChattingForm() {
	this->chattingView = NULL;
	this->chattingEdit = NULL;
	this->chatter = NULL;
}

int ChattingForm::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	CFrameWnd::OnCreate(lpCreateStruct);

	//Position of Windows
	CRect rect;
	this->GetClientRect(rect);

	rect.left += MARGIN;
	rect.top += MARGIN;
	rect.right -= MARGIN;
	rect.bottom -= MARGIN;

	Long sendButtonWidth = rect.Width() / 8;

	CRect editRect(rect);
	editRect.top += editRect.bottom - 50;
	editRect.right -= (sendButtonWidth + MARGIN);

	CRect viewRect(rect);
	viewRect.bottom -= (editRect.Height() + MARGIN);

	//ChattingView
	this->chattingView = new TextEditingForm;
	this->chattingView->Create(NULL, "CNTextEditor", WS_CHILD,
		viewRect, this, NULL, NULL);
	this->chattingView->ShowWindow(SW_SHOW);
	this->chattingView->UpdateWindow();

	//ChattingEdit
	this->chattingEdit = new TextEditingForm;
	this->chattingEdit->Create(NULL, "CNTextEditor", WS_CHILD,
		editRect, this, NULL, NULL);
	this->chattingEdit->ShowWindow(SW_SHOW);
	this->chattingEdit->UpdateWindow();
	this->chattingEdit->SetFocus();

	return 0;
}

void ChattingForm::OnClose() {
	if (this->chattingView != NULL) {
		delete this->chattingView;
	}
	if (this->chattingEdit != NULL) {
		delete this->chattingEdit;
	}
	if (this->chatter != NULL) {
		delete this->chatter;
	}

	CFrameWnd::OnClose();
}

void ChattingForm::OnPaint() {

}

void ChattingForm::OnSize(UINT nType, int cx, int cy) {
	CFrameWnd::OnSize(nType, cx, cy);
}

void ChattingForm::OnSetFocus(CWnd* pOldWnd) {
	CFrameWnd::OnSetFocus(pOldWnd);
	if (this->chattingEdit != NULL) {
		this->chattingEdit->SetFocus();
	}
}

void ChattingForm::OnKillFocus(CWnd* pNewWnd) {
	CFrameWnd::OnKillFocus(pNewWnd);
}

void ChattingForm::OnCommandRange(UINT uID) {

}