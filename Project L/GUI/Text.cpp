#include "Text.h"

#include "../Utils/Error.h"

Text::Text() : color(0.0f, 0.0f, 0.0f, 1.0f)
{
	this->font = nullptr;
}

Text::Text(const std::string & text, Font* font) : color(0.0f, 0.0f, 0.0f, 1.0f)
{
	setText(text, font);
}

Text::~Text()
{
}

void Text::setColor(const Vec4 & color)
{
	this->color = color;
}

void Text::setText(const std::string & text, Font* font)
{
	if (font != nullptr)
		this->font = font;
	if (this->font == nullptr)
	{
		Error::printWarning("TEXT::setText()", "Font is missing!");
		return;
	}

	this->characterRects.clear();
	this->text = text;

	this->width = 0.0f;
	this->height = 0.0f;

	float x = 0.0f;
	const char* p;
	for (p = text.c_str(); *p; p++)
	{
		Font::Character character = this->font->getCharacter(*p);

		float x2 = x + character.bearingX;
		float y2 = character.bearingY;
		float w = character.width;
		float h = character.height;

		this->width += x2 + w;
		if (this->height < h)
			this->height = h;
		if (this->bearingY < y2)
			this->bearingY = y2;

		CharacterRect rect;
		rect.textureID = character.textureID;
		rect.rect.tl = { x2,	 y2,	 0, 0 };
		rect.rect.tr = { x2 + w, y2,	 1, 0 };
		rect.rect.bl = { x2,	 y2 - h, 0, 1 };
		rect.rect.br = { x2 + w, y2 - h, 1, 1 };
		this->characterRects.push_back(rect);

		x += (character.advance >> 6);
	}
}

Vec4 Text::getColor() const
{
	return this->color;
}

float Text::getWidth() const
{
	return this->width;
}

float Text::getHeight() const
{
	return this->height;
}

float Text::getBearingY() const
{
	return this->bearingY;
}

std::vector<Text::CharacterRect> Text::getCharacterRects()
{
	return this->characterRects;
}
