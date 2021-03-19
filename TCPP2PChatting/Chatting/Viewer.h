#ifndef _VIEWER_H
#define _VIEWER_H

#include <string>

using namespace std;

class ChattingForm;

class Viewer {
public:
	Viewer(ChattingForm *chattingForm = 0);
	~Viewer();
	Viewer(const Viewer& source);
	Viewer& operator=(const Viewer& source);
	
	void View(string content);
private:
	ChattingForm* chattingForm;
};

#endif //_VIEWER_H