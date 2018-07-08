#include "TextRenderer.h"

#include "../Utils/Error.h"
#include "../Maths/Vectors/Vec4.h"
#include "../Maths/Vectors/Vec2.h"

TextRenderer::TextRenderer() : font(nullptr)
{
	init();
}

TextRenderer::TextRenderer(const std::string & text, Font* font) : font(font)
{
	init();
}

TextRenderer::~TextRenderer()
{
}

void TextRenderer::setFont(Font * font)
{
	this->font = font;
}

void TextRenderer::renderText(Text & text, float x, float y, float scale, Display * display)
{
	std::vector<Text::CharacterRect> characterRects = text.getCharacterRects();
	const float scaleX = 2.f*display->getPixelXScale() * scale;
	const float scaleY = 2.f*display->getPixelYScale() * scale;
	for (unsigned int i = 0; i < characterRects.size(); i++)
	{
		Text::CharacterRect rect = characterRects[i];
		rect.rect.tl.x = rect.rect.tl.x*scaleX + x;
		rect.rect.tl.y = rect.rect.tl.y*scaleY + y;
		rect.rect.tr.x = rect.rect.tr.x*scaleX + x;
		rect.rect.tr.y = rect.rect.tr.y*scaleY + y;
		rect.rect.bl.x = rect.rect.bl.x*scaleX + x;
		rect.rect.bl.y = rect.rect.bl.y*scaleY + y;
		rect.rect.br.x = rect.rect.br.x*scaleX + x;
		rect.rect.br.y = rect.rect.br.y*scaleY + y;

		this->vb.updateData(&rect.rect.tl[0], sizeof(Text::CharacterRect::Rect));

		this->font->getShader()->bind();
		this->font->getShader()->setUniform4fv("color", 1, &text.getColor()[0]);
		glBindTexture(GL_TEXTURE_2D, rect.textureID);
		glActiveTexture(GL_TEXTURE0);
		this->font->getShader()->setTexture2D("tex", 0, rect.textureID);

		this->va.bind();
		this->vb.bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
}

void TextRenderer::renderText(const char* text, float x, float y, float scale, Display* display)
{
	if (this->font == nullptr)
	{
		Error::printError("TEXT::setText()", "Text has no font!");
	}

	const char* p;
	for (p = text; *p; p++)
	{
		Font::Character character = this->font->getCharacter(*p);
		
		float x2 = x + character.bearingX * display->getPixelXScale() * scale * 2.f;
		float y2 = y + character.bearingY * display->getPixelYScale() * scale * 2.f;
		float w = character.width * display->getPixelXScale() * scale * 2.f;
		float h = character.height * display->getPixelYScale() * scale * 2.f;
		
		GLfloat rect[4][4] = {
			{ x2,     y2    , 0, 0 },
			{ x2 + w, y2    , 1, 0 },
			{ x2,     y2 - h, 0, 1 },
			{ x2 + w, y2 - h, 1, 1 },
		};
		float color[4] = { 0.0, 0.0, 0.0, 1.0 };
		this->vb.updateData(&rect[0][0], sizeof(rect));

		this->font->getShader()->bind();
		this->font->getShader()->setUniform4fv("color", 1, color);
		glBindTexture(GL_TEXTURE_2D, character.textureID);
		glActiveTexture(GL_TEXTURE0);
		this->font->getShader()->setTexture2D("tex", 0, character.textureID);

		this->va.bind();
		this->vb.bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		x += (character.advance >> 6) * display->getPixelXScale() * scale * 2.f;
	}
}

void TextRenderer::init()
{
	this->vb.make(NULL, sizeof(float) * 16, GL_DYNAMIC_DRAW);
	VertexBufferLayout layout;
	layout.push<float>(4); // Pos and uv.
	this->va.addBuffer(this->vb, layout);
}
