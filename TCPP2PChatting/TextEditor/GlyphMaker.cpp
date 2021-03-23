#include "GlyphMaker.h"
#include "Scanner.h"
#include "GlyphFactory.h"
#include "Glyph.h"

GlyphMaker::GlyphMaker() {

}

GlyphMaker::GlyphMaker(const GlyphMaker& source) {

}

GlyphMaker::~GlyphMaker() {

}

GlyphMaker& GlyphMaker::operator=(const GlyphMaker& source) {
	return *this;
}

Glyph* GlyphMaker::Make(string content) {
	GlyphFactory glyphFactory;
	Glyph* note = glyphFactory.Make("");
	Glyph* line = glyphFactory.Make("\r\n");
	note->Add(line);
	Glyph* glyph;
	string token;
	Scanner scanner(content);
	while (scanner.IsEnd() == false) {
		token = scanner.GetToken();
		if (token != "\r\n") {
			glyph = glyphFactory.Make(token.c_str());
			line->Add(glyph);
		}
		else {
			glyph = glyphFactory.Make(token.c_str());
			Long index = note->Add(glyph);
			line = note->GetAt(index);
		}
		scanner.Next();
	}

	return note;
}