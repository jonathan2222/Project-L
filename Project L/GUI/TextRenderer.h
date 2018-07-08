#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <string>

#include "Font.h"
#include "../GLAbstractions/Texture.h"
#include "../GLAbstractions/VertexArray.h"
#include "Text.h"

class TextRenderer
{
public:
	TextRenderer();
	TextRenderer(const std::string& text, Font* font);
	~TextRenderer();

	// Set font if you don't want to use the Text class.
	void setFont(Font* font);

	void renderText(Text& text, float x, float y, float scale, Display* display);
	void renderText(const char* text, float x, float y, float scale, Display* display);

private:
	void init();

	Font* font;
	std::string text;

	VertexArray va;
	VertexBuffer vb;
};

#endif
