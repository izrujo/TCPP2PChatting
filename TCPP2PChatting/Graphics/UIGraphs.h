#ifndef _UIGRAPHS_H
#define _UIGRAPHS_H

#define WINDOWCAPTION 901
#define WINDOWCLOSEBUTTON 902
#define NUMBERSTICK 903
#define LINENUMBER 904
#define BUTTON 905

#include "../Utilities/Array.h"

#include <string>
using namespace std;


class GraphVisitor;

class UIGraph {
public:
	UIGraph();
	UIGraph(Long x, Long y, Long width, Long height, string content);
	UIGraph(const UIGraph& source);
	virtual ~UIGraph() = 0;
	UIGraph& operator=(const UIGraph& source);

	virtual UIGraph* Clone() = 0;
	virtual void Accept(GraphVisitor* visitor) = 0;

	virtual Long GetX() const;
	virtual Long GetY() const;
	virtual Long GetWidth() const;
	virtual Long GetHeight() const;
	virtual string GetContent() const;

	virtual Long Add(UIGraph* uiGraph);
	virtual Long Add(Long index, UIGraph* uiGraph);
	virtual Long Remove(Long index);
	virtual UIGraph* GetAt(Long index);

	virtual Long GetCapacity() const;
	virtual Long GetLength() const;

protected:
	Long x;
	Long y;
	Long width;
	Long height;
	string content;
};

inline Long UIGraph::GetX() const {
	return this->x;
}

inline Long UIGraph::GetY() const {
	return this->y;
}

inline Long UIGraph::GetWidth() const {
	return this->width;
}

inline Long UIGraph::GetHeight() const {
	return this->height;
}

inline string UIGraph::GetContent() const {
	return this->content;
}

//UIGraphComposite
class UIGraphComposite : public UIGraph {
public:
	UIGraphComposite(Long capacity = 256);
	UIGraphComposite(const UIGraphComposite& source);
	virtual ~UIGraphComposite() = 0;
	UIGraphComposite& operator=(const UIGraphComposite& source);
	
	virtual Long Add(UIGraph* uiGraph);
	virtual Long Add(Long index, UIGraph* uiGraph);
	virtual Long Remove(Long index);
	virtual UIGraph* GetAt(Long index);

	virtual Long GetCapacity() const;
	virtual Long GetLength() const;

protected:
	Array<UIGraph*> uiGraphs;
	Long capacity;
	Long length;
};

inline Long UIGraphComposite::GetCapacity() const {
	return this->capacity;
}

inline Long UIGraphComposite::GetLength() const {
	return this->length;
}

//NumberStick
class NumberStick : public UIGraphComposite {
public:
	NumberStick(Long capacity = 256);
	NumberStick(const NumberStick& source);
	virtual ~NumberStick();
	NumberStick& operator=(const NumberStick& source);

	virtual UIGraph* Clone();
	virtual void Accept(GraphVisitor* visitor);
};

//WindowCaption
class WindowCaption : public UIGraph {
public:
	WindowCaption(Long x, Long y, Long width, Long height, string content);
	WindowCaption(const WindowCaption& source);
	virtual ~WindowCaption();
	WindowCaption& operator=(const WindowCaption& source);

	virtual UIGraph* Clone();
	virtual void Accept(GraphVisitor* visitor);
};

//WindowCloseButton
class WindowCloseButton : public UIGraph {
public:
	WindowCloseButton(Long x, Long y, Long width, Long height, string content);
	WindowCloseButton(const WindowCloseButton& source);
	virtual ~WindowCloseButton();
	WindowCloseButton& operator=(const WindowCloseButton& source);

	virtual UIGraph* Clone();
	virtual void Accept(GraphVisitor* visitor);
};

//LineNumber
class LineNumber : public UIGraph {
public:
	LineNumber(Long x, Long y, Long width, Long height, string content);
	LineNumber(const LineNumber& source);
	virtual ~LineNumber();
	LineNumber& operator=(const LineNumber& source);

	virtual UIGraph* Clone();
	virtual void Accept(GraphVisitor* visitor);
};

//Button
class Button : public UIGraph {
public:
	Button(Long x, Long y, Long width, Long height, string content);
	Button(const Button& source);
	virtual ~Button();
	Button& operator=(const Button& source);

	virtual UIGraph* Clone();
	virtual void Accept(GraphVisitor* visitor);
};

#endif //_UIGRAPHS_H