#ifndef _UIGRAPHFACTORY_H
#define _UIGRAPHFACTORY_H

#include "UIGraphs.h"

class UIGraphFactory {
public:
	UIGraphFactory();
	UIGraphFactory(const UIGraphFactory& source);
	~UIGraphFactory();
	UIGraphFactory& operator=(const UIGraphFactory& source);

	UIGraph* Make(int id, Long x, Long y, Long width, Long height, string content = "");
};

#endif //_UIGRAPHFACTORY_H