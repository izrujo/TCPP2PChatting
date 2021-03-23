#ifndef _GLYPHMAKER_H
#define _GLYPHMAKER_H

#include <string>
using namespace std;

class Glyph;

class GlyphMaker {
public:
	GlyphMaker();
	GlyphMaker(const GlyphMaker& source);
	~GlyphMaker();
	GlyphMaker& operator=(const GlyphMaker& source);

	Glyph* Make(string content);
};

#endif //_GLYPHMAKER_H