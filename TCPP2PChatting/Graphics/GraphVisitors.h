#ifndef _GRAPHVISITORS_H
#define _GRAPHVISITORS_H

#include <afxwin.h>

class ScrollController;

class WindowCaption;
class WindowCloseButton;
class NumberStick;
class LineNumber;
class Button;

class GraphVisitor {
public:
	GraphVisitor();
	virtual ~GraphVisitor() = 0;

	virtual void Visit(WindowCaption* element) = 0;
	virtual void Visit(WindowCloseButton* element) = 0;
	virtual void Visit(NumberStick* element) = 0;
	virtual void Visit(LineNumber* element) = 0;
	virtual void Visit(Button* element) = 0;
};

//GraphDrawingVisitor
class GraphDrawingVisitor : public GraphVisitor {
public:
	GraphDrawingVisitor(CDC* dc = 0, ScrollController* scrollController = 0);
	virtual ~GraphDrawingVisitor();

	virtual void Visit(WindowCaption* element);
	virtual void Visit(WindowCloseButton* element);
	virtual void Visit(NumberStick* element);
	virtual void Visit(LineNumber* element);
	virtual void Visit(Button* element);

private:
	CDC* dc;
	ScrollController* scrollController;
};

#endif //_GRAPHVISITORS_H