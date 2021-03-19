#include "GraphVisitors.h"
#include "UIGraphs.h"
#include "../TextEditor/ScrollController.h"
#include "../TextEditor/Scroll.h"

GraphVisitor::GraphVisitor() {

}

GraphVisitor::~GraphVisitor() {

}

//GraphDrawingVisitor
GraphDrawingVisitor::GraphDrawingVisitor(CDC* dc, ScrollController* scrollController) {
	this->dc = dc;
	this->scrollController = scrollController;
}

GraphDrawingVisitor::~GraphDrawingVisitor() {

}

void GraphDrawingVisitor::Visit(WindowCaption* element) {
	Long x = element->GetX();
	Long y = element->GetY();
	Long width = element->GetWidth();
	Long height = element->GetHeight();

	COLORREF bkColor = this->dc->GetBkColor();
	COLORREF textColor = this->dc->GetTextColor();

	this->dc->FillSolidRect(x, y, width, height, bkColor);

	CPen pen(PS_SOLID, 2, textColor);
	CPen* oldPen = this->dc->SelectObject(&pen);

	this->dc->MoveTo(x, y);
	this->dc->LineTo(x + width, y);
	this->dc->MoveTo(x, y + height);
	this->dc->LineTo(x + width, y + height);

	this->dc->SelectObject(oldPen);

	CFont* font = this->dc->GetCurrentFont();
	LOGFONT logFont;
	font->GetLogFont(&logFont);
	Long contentHeight = logFont.lfHeight;
	CRect rect(x + 10, y + (height - contentHeight) / 2, x + width, y + height);
	this->dc->DrawText(element->GetContent().c_str(), rect, DT_LEFT | DT_VCENTER);
}

void GraphDrawingVisitor::Visit(WindowCloseButton* element) {
	Long x = element->GetX();
	Long y = element->GetY();
	Long width = element->GetWidth();
	Long height = element->GetHeight();
	float hhhWidth = width / 8.0F; //half of half of half of width
	float hhhHeight = height / 8.0F;

	COLORREF bkColor = this->dc->GetBkColor();
	COLORREF textColor = this->dc->GetTextColor();

	this->dc->FillSolidRect(x, y, width, height, bkColor);

	CPen pen(PS_SOLID, 2, textColor);
	CPen* oldPen = this->dc->SelectObject(&pen);

	this->dc->MoveTo(x, y);
	this->dc->LineTo(x + width, y);
	this->dc->MoveTo(x, y + height);
	this->dc->LineTo(x + width, y + height);

	this->dc->MoveTo(x + hhhWidth * 2, y + hhhHeight * 2);
	this->dc->LineTo(x + hhhWidth * 6, y + hhhHeight * 6);

	this->dc->MoveTo(x + hhhWidth * 6, y + hhhHeight * 2);
	this->dc->LineTo(x + hhhWidth * 2, y + hhhHeight * 6);

	this->dc->SelectObject(oldPen);
}

void GraphDrawingVisitor::Visit(NumberStick* element) {
	Long i = 0;
	while (i < element->GetLength()) {
		element->GetAt(i)->Accept(this);
		i++;
	}
}

void GraphDrawingVisitor::Visit(LineNumber* element) {
	Long x = element->GetX();
	Long y = element->GetY();
	Long width = element->GetWidth();
	Long height = element->GetHeight();

	Long yScrollPosition = 0;
	if (this->scrollController != NULL) {
		yScrollPosition = this->scrollController->GetVerticalScroll()->GetPosition();
	}
	y -= yScrollPosition;

	CFont* font = this->dc->GetCurrentFont();
	LOGFONT logFont;
	font->GetLogFont(&logFont);
	logFont.lfHeight = height;
	Long contentHeight = logFont.lfHeight;
	CRect rect(x, y + (height - contentHeight) / 2, x + width - 10, y + height);
	this->dc->DrawText(element->GetContent().c_str(), rect, DT_RIGHT | DT_VCENTER);
}

void GraphDrawingVisitor::Visit(Button* element) {
	Long x = element->GetX();
	Long y = element->GetY();
	Long width = element->GetWidth();
	Long height = element->GetHeight();

	this->dc->Rectangle(x, y, x + width, y + height);

	COLORREF oldBkColor = this->dc->GetBkColor();
	this->dc->SetBkColor(RGB(255, 255, 255));
	CFont* font = this->dc->GetCurrentFont();
	LOGFONT logFont;
	font->GetLogFont(&logFont);
	logFont.lfHeight = height;
	Long contentHeight = logFont.lfHeight;
	CRect rect(x, y + height / 4, x + width, y + height);
	this->dc->DrawText(element->GetContent().c_str(), rect, DT_CENTER);
	this->dc->SetBkColor(oldBkColor);
}

//void GraphDrawingVisitor::Visit(UIGraph* element) {
//	함수 추가 복사용
//}