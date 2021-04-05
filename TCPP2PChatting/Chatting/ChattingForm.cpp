#include "ChattingForm.h"
#include "Viewer.h"
#include "CommandFactory.h"
#include "Commands.h"
#include "ChatEditingForm.h"
#include "Chatter.h"
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "Packet.h"

#include "resource.h"

#include "../TextEditor/TextEditingForm.h"
#include "../TextEditor/GlyphMaker.h"
#include "../TextEditor/Glyph.h"

#include "../Graphics/UIGraphFactory.h"
#include "../Graphics/GraphVisitors.h"

#pragma warning(disable:4996)

#define MARGIN 10

BEGIN_MESSAGE_MAP(ChattingForm, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONUP()
	ON_COMMAND_RANGE(IDC_SEND_MESSAGE, IDC_SEND_MESSAGE, OnCommandRange)
END_MESSAGE_MAP()

ChattingForm::ChattingForm() {
	this->chattingView = NULL;
	this->chattingEdit = NULL;
	this->chatter = NULL;
	this->sendButton = NULL;

	this->ipAddress = "121.138.194.196";//"14.32.96.30";
	this->portNumber = 2180;
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
	editRect.top += editRect.bottom - 150;
	editRect.right -= (sendButtonWidth + MARGIN);

	CRect viewRect(rect);
	viewRect.bottom -= (editRect.Height() + MARGIN);

	CRect sendButtonRect(rect);
	sendButtonRect.top += sendButtonRect.bottom - 150;
	sendButtonRect.bottom -= 100;
	sendButtonRect.left += (editRect.Width() + MARGIN);

	//ChattingView
	this->chattingView = new TextEditingForm;
	this->chattingView->Create(NULL, "CNTextEditor", WS_CHILD | WS_BORDER,
		viewRect, this, NULL, NULL);
	this->chattingView->ShowWindow(SW_SHOW);
	this->chattingView->UpdateWindow();

	//ChattingEdit
	this->chattingEdit = new ChatEditingForm;
	this->chattingEdit->Create(NULL, "CNTextEditor", WS_CHILD | WS_BORDER,
		editRect, this, NULL, NULL);
	this->chattingEdit->ShowWindow(SW_SHOW);
	this->chattingEdit->UpdateWindow();
	this->chattingEdit->SetFocus();

	//SendButton
	UIGraphFactory uiFactory;
	this->sendButton = uiFactory.Make(BUTTON, sendButtonRect.left, sendButtonRect.top,
		sendButtonRect.Width(), sendButtonRect.Height(), "Send");

	this->chatter = new Chatter(this);

	this->chatter->Listen();

	// 콜을 임시 연결로 수정하고 accept에 실제로 연결을 처리한다. 연결할땐 내부, 만들땐 외부
	//this->chatter->Call(this->ipAddress.c_str(), this->portNumber);

	return 0;
}

void ChattingForm::OnClose() {
	// 1. 종료 버튼을 클릭했을 때
	// 2. 종료가 허가되면 종료한다.
	if (this->chatter->onIsFinished == TRUE) {
		if (this->chattingView != NULL) {
			delete this->chattingView;
			this->chattingView = NULL;
		}
		if (this->chattingEdit != NULL) {
			delete this->chattingEdit;
			this->chattingEdit = NULL;
		}
		if (this->chatter != NULL) {
			delete this->chatter;
		}

		CFrameWnd::OnClose();
	}
	// 3. 종료가 허가되지 않으면
	else {
		// 3.1. 종료 패킷을 만든다.
		CString socketAddress;
		socketAddress.Format("%s:%d", (LPCTSTR)this->chatter->serverSocket.ipAddress, this->chatter->serverSocket.portNumber);
		Packet* packet = new Packet(0, Packet::ID_FINISH, "");

		// 3.2. 모든 클라이언트들에게 송신하다.
		this->chatter->serverSocket.SendDataAll(packet);

		if (packet != 0) {
			delete packet;
		}
		this->chatter->onIsFinished = TRUE;
	}
}

void ChattingForm::OnPaint() {
	CPaintDC dc(this);

	CRect rect;
	this->GetClientRect(rect);

	dc.FillSolidRect(rect, RGB(235, 235, 235));

	GraphVisitor* visitor = new GraphDrawingVisitor(&dc);
	this->sendButton->Accept(visitor);
}

void ChattingForm::OnSize(UINT nType, int cx, int cy) {
	//Position of Windows
	CRect rect;
	this->GetClientRect(rect);

	rect.left += MARGIN;
	rect.top += MARGIN;
	rect.right -= MARGIN;
	rect.bottom -= MARGIN;

	Long sendButtonWidth = rect.Width() / 8;

	CRect editRect(rect);
	editRect.top += editRect.bottom - 150;
	editRect.right -= (sendButtonWidth + MARGIN);

	CRect viewRect(rect);
	viewRect.bottom -= (editRect.Height() + MARGIN);

	CRect sendButtonRect(rect);
	sendButtonRect.top += sendButtonRect.bottom - 150;
	sendButtonRect.bottom -= 100;
	sendButtonRect.left += (editRect.Width() + MARGIN);

	if (this->chattingView != NULL) {
		this->chattingView->MoveWindow(viewRect);
	}

	if (this->chattingEdit != NULL) {
		this->chattingEdit->MoveWindow(editRect);
	}

	if (this->sendButton != NULL) {
		delete this->sendButton;
		UIGraphFactory uiFactory;
		this->sendButton = uiFactory.Make(BUTTON, sendButtonRect.left, sendButtonRect.top,
			sendButtonRect.Width(), sendButtonRect.Height(), "Send");
	}

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
	CommandFactory commandFactory(this);
	Command* command = commandFactory.Make(uID);
	if (command != NULL) {
		command->Execute();
		delete command;
	}
}

void ChattingForm::OnLButtonUp(UINT nFlags, CPoint point) {
	Long x = this->sendButton->GetX();
	Long y = this->sendButton->GetY();
	Long width = this->sendButton->GetWidth();
	Long height = this->sendButton->GetHeight();

	if (point.x > x && point.x < x + width && point.y > y && point.y < y + height) {
		//AfxMessageBox(_T("Send"));
		this->SendMessage(WM_COMMAND, MAKEWPARAM(IDC_SEND_MESSAGE, 0));
	}
}