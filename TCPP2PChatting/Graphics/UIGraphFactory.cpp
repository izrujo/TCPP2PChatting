#include "UIGraphFactory.h"

UIGraphFactory::UIGraphFactory() {

}

UIGraphFactory::UIGraphFactory(const UIGraphFactory& source) {

}

UIGraphFactory::~UIGraphFactory() {

}

UIGraphFactory& UIGraphFactory::operator=(const UIGraphFactory& source) {
	return *this;
}

UIGraph* UIGraphFactory::Make(int id, Long x, Long y, Long width, Long height, string content) {
	UIGraph* uiGraph = 0;
	if (id == WINDOWCAPTION) {
		uiGraph = new WindowCaption(x, y, width, height, content);
	}
	else if (id == WINDOWCLOSEBUTTON) {
		uiGraph = new WindowCloseButton(x, y, width, height, content);
	}
	else if (id == LINENUMBER) {
		uiGraph = new LineNumber(x, y, width, height, content);
	}
	else if (id == BUTTON) {
		uiGraph = new Button(x, y, width, height, content);
	}

	return uiGraph;
}