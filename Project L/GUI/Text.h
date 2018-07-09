#ifndef TEXT_H
#define TEXT_H

#include <string>
#include "Font.h"
#include "../Maths/Vectors/Vec4.h"

class Text
{
public:
	struct CharacterRect
	{
		GLuint textureID;
		struct Rect
		{
			Vec4 tl;
			Vec4 tr;
			Vec4 bl;
			Vec4 br;
		} rect;
	};
public:
	Text();
	Text(const std::string& text, Font* font);
	~Text();

	void setColor(const Vec4& color);
	void setText(const std::string & text, Font* font = nullptr);

	Vec4 getColor() const;
	float getWidth() const; // In bitmap pixels.
	float getHeight() const; // In bitmap pixels.
	float getBearingY() const; // In bitmap pixels.


	std::vector<CharacterRect> getCharacterRects();

private:
	std::string text;
	float width;
	float height;
	float bearingY;
	Vec4 color;

	Font* font;
	std::vector<CharacterRect> characterRects;
};

#endif
