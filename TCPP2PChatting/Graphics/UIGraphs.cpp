#include "UIGraphs.h"
#include "GraphVisitors.h"

UIGraph::UIGraph() {
	this->x = -1;
	this->y = -1;
	this->width = -1;
	this->height = -1;
	this->content = "";
}

UIGraph::UIGraph(Long x, Long y, Long width, Long height, string content) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->content = content;
}

UIGraph::UIGraph(const UIGraph& source) {
	this->x = source.x;
	this->y = source.y;
	this->width = source.width;
	this->height = source.height;
	this->content = source.content;
}

UIGraph::~UIGraph() {

}

UIGraph& UIGraph::operator=(const UIGraph& source) {
	this->x = source.x;
	this->y = source.y;
	this->width = source.width;
	this->height = source.height;
	this->content = source.content;

	return *this;
}

Long UIGraph::Add(UIGraph* uiGraph) {
	return -1;
}

Long UIGraph::Add(Long index, UIGraph* uiGraph) {
	return -1;
}

Long UIGraph::Remove(Long index) {
	return -1;
}

UIGraph* UIGraph::GetAt(Long index) {
	return 0;
}

Long UIGraph::GetCapacity() const {
	return -1;
}

Long UIGraph::GetLength() const {
	return -1;
}

//UIGraphComposite
UIGraphComposite::UIGraphComposite(Long capacity) 
	: uiGraphs(capacity) {
	this->capacity = capacity;
	this->length = 0;
}

UIGraphComposite::UIGraphComposite(const UIGraphComposite& source) 
	: uiGraphs(source.uiGraphs) {
	Long i = 0;
	while (i < source.length) {
		this->uiGraphs.Modify(i, (const_cast<UIGraphComposite&>(source)).uiGraphs[i]->Clone());
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;
}

UIGraphComposite::~UIGraphComposite() {
	Long i = 0;
	while (i < this->length) {
		if (this->uiGraphs[i] != 0) {
			delete this->uiGraphs[i];
		}
		i++;
	}
}

UIGraphComposite& UIGraphComposite::operator=(const UIGraphComposite& source) {
	Long i = 0;
	while (i < this->length) {
		if (this->uiGraphs[i] != 0) {
			delete this->uiGraphs[i];
		}
		i++;
	}
	this->uiGraphs = source.uiGraphs;
	i = 0;
	while (i < source.length) {
		this->uiGraphs.Modify(i, (const_cast<UIGraphComposite&>(source)).uiGraphs[i]->Clone());
		i++;
	}
	this->capacity = source.capacity;
	this->length = source.length;

	return *this;
}

Long UIGraphComposite::Add(UIGraph* uiGraph) {
	Long index;
	if (this->length < this->capacity) {
		index = this->uiGraphs.Store(this->length, uiGraph);
	}
	else {
		index = this->uiGraphs.AppendFromRear(uiGraph);
		this->capacity++;
	}
	this->length++;

	return index;
}

Long UIGraphComposite::Add(Long index, UIGraph* uiGraph) {
	index = this->uiGraphs.Insert(index, uiGraph);
	this->length++;
	this->capacity++;

	return index;
}

Long UIGraphComposite::Remove(Long index) {
	if (this->uiGraphs[index] != 0) {
		delete this->uiGraphs.GetAt(index);
	}
	index = this->uiGraphs.Delete(index);
	this->capacity--;
	this->length--;

	return index;
}

UIGraph* UIGraphComposite::GetAt(Long index) {
	return this->uiGraphs.GetAt(index);
}

//NumberStick
NumberStick::NumberStick(Long capacity)
	: UIGraphComposite(capacity) {

}

NumberStick::NumberStick(const NumberStick& source) 
	: UIGraphComposite(source) {

}

NumberStick::~NumberStick() {

}

NumberStick& NumberStick::operator=(const NumberStick& source) {
	UIGraphComposite::operator=(source);

	return *this;
}

UIGraph* NumberStick::Clone() {
	return new NumberStick(*this);
}

void NumberStick::Accept(GraphVisitor* visitor) {
	visitor->Visit(this);
}

//WindowCaption
WindowCaption::WindowCaption(Long x, Long y, Long width, Long height, string content)
	: UIGraph(x, y, width, height, content) {

}

WindowCaption::WindowCaption(const WindowCaption& source)
	: UIGraph(source) {

}

WindowCaption::~WindowCaption() {

}

WindowCaption& WindowCaption::operator=(const WindowCaption& source) {
	UIGraph::operator=(source);

	return *this;
}

UIGraph* WindowCaption::Clone() {
	return new WindowCaption(*this);
}

void WindowCaption::Accept(GraphVisitor* visitor) {
	visitor->Visit(this);
}

//WindowCloseButton
WindowCloseButton::WindowCloseButton(Long x, Long y, Long width, Long height, string content)
	: UIGraph(x, y, width, height, content) {

}

WindowCloseButton::WindowCloseButton(const WindowCloseButton& source)
	: UIGraph(source) {

}

WindowCloseButton::~WindowCloseButton() {

}

WindowCloseButton& WindowCloseButton::operator=(const WindowCloseButton& source) {
	UIGraph::operator=(source);

	return *this;
}

UIGraph* WindowCloseButton::Clone() {
	return new WindowCloseButton(*this);
}

void WindowCloseButton::Accept(GraphVisitor* visitor) {
	visitor->Visit(this);
}

//LineNumber
LineNumber::LineNumber(Long x, Long y, Long width, Long height, string content)
	: UIGraph(x, y, width, height, content) {

}

LineNumber::LineNumber(const LineNumber& source)
	: UIGraph(source) {

}

LineNumber::~LineNumber() {

}

LineNumber& LineNumber::operator=(const LineNumber& source) {
	UIGraph::operator=(source);

	return *this;
}

UIGraph* LineNumber::Clone() {
	return new LineNumber(*this);
}

void LineNumber::Accept(GraphVisitor* visitor) {
	visitor->Visit(this);
}

//Button
Button::Button(Long x, Long y, Long width, Long height, string content)
	: UIGraph(x, y, width, height, content) {

}

Button::Button(const Button& source)
	: UIGraph(source) {

}

Button::~Button() {

}

Button& Button::operator=(const Button& source) {
	UIGraph::operator=(source);

	return *this;
}

UIGraph* Button::Clone() {
	return new Button(*this);
}

void Button::Accept(GraphVisitor* visitor) {
	visitor->Visit(this);
}