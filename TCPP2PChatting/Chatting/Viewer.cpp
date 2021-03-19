#include "Viewer.h"
#include "ChattingForm.h"

#include "../TextEditor/TextEditingForm.h"
#include "../TextEditor/Scanner.h"
#include "../TextEditor/GlyphFactory.h"
#include "../TextEditor/Glyph.h"
#include "../TextEditor/CharacterMetrics.h"
#include "../TextEditor/ScrollController.h"


Viewer::Viewer(ChattingForm* chattingForm) {
	this->chattingForm = chattingForm;
}

Viewer::~Viewer() {

}

Viewer::Viewer(const Viewer& source) {
	this->chattingForm = source.chattingForm;
}

Viewer& Viewer::operator=(const Viewer& source) {
	this->chattingForm = source.chattingForm;

	return *this;
}

void Viewer::View(string content) {
	GlyphFactory glyphFactory;
	Glyph* glyph;
	string token;
	Scanner scanner(content);
	while (scanner.IsEnd() == FALSE) {
		token = scanner.GetToken();
		if (token != "\r\n") {
			glyph = glyphFactory.Make(token.c_str());
			this->chattingForm->chattingView->current->Add(glyph);
		}
		else {
			glyph = glyphFactory.Make(token.c_str());
			Long index = this->chattingForm->chattingView->note->Add(glyph);
			this->chattingForm->chattingView->current = this->chattingForm->chattingView->note->GetAt(index);
		}
		scanner.Next();
	}

	this->chattingForm->chattingView->Invalidate();
	this->chattingForm->chattingView->Notify();

	Long x = this->chattingForm->chattingView->
		characterMetrics->GetX(this->chattingForm->chattingView->current) + 1; // 
	Long y = this->chattingForm->chattingView->
		characterMetrics->GetY(this->chattingForm->chattingView->note->GetCurrent() + 1); // 0베이스이므로 1더함
	this->chattingForm->chattingView->scrollController->SmartScrollToPoint(x, y);
}