#include "Font.h"

#include "../Utils/Error.h"

Shader * Font::shader = nullptr;

Font::Font()
{
	if(Font::shader == nullptr)
		Font::shader = new Shader("Shaders/Font");
}

Font::Font(const std::string & fontPath, unsigned int size, Display* display)
{
	if (Font::shader == nullptr)
		Font::shader = new Shader("Shaders/Font");
	load(fontPath, size, display);
}

Font::~Font()
{
	freeCharacters();
	FT_Done_Face(this->face);
}

bool Font::load(const std::string & fontPath, unsigned int size, Display* display)
{
	if (FT_New_Face(display->getFTLibrary(), fontPath.c_str(), 0, &this->face))
	{
		Error::printError("FONT::load()", "Could not load font: '" + fontPath + "'");
		return false;
	}
	FT_Set_Pixel_Sizes(this->face, 0, size);

	freeCharacters();
	loadCharacters();
	return true;
}

void Font::setSize(unsigned int size)
{
	FT_Set_Pixel_Sizes(this->face, 0, size);
	updateCharacters();
}

Shader * Font::getShader()
{
	return Font::shader;
}

Font::Character& Font::getCharacter(char c)
{
	return this->characters[c];
}

std::unordered_map<char, Font::Character>& Font::getCharacters()
{
	return this->characters;
}

void Font::FREE_SHADER()
{
	if (Font::shader != nullptr)
	{
		delete Font::shader;
		Font::shader = nullptr;
	}
}

void Font::freeCharacters()
{
	std::unordered_map<char, Character>::iterator it;
	for (it = this->characters.begin(); it != this->characters.end(); it++)
		glDeleteTextures(1, &it->second.textureID);
}

void Font::loadCharacters()
{
	// Might increase this later if needed.
	for (GLubyte c = 0; c < 128; c++)
	{
		if (FT_Load_Char(this->face, c, FT_LOAD_RENDER))
		{
			Error::printWarning("Could not load character '" + std::string(1, c) + "'!");
			continue;
		}
		Character character;
		character.width = this->face->glyph->bitmap.width;
		character.height = this->face->glyph->bitmap.rows;
		character.bearingX = this->face->glyph->bitmap_left;
		character.bearingY = this->face->glyph->bitmap_top;
		character.advance = this->face->glyph->advance.x;

		GLuint texId;
		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, character.width, character.height, 0, GL_RED, GL_UNSIGNED_BYTE, this->face->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glBindTexture(GL_TEXTURE_2D, 0);
		character.textureID = texId;

		this->characters.insert(std::pair<GLchar, Character>(c, character));
	}
}

void Font::updateCharacters()
{
	// Might increase this later if needed.
	for (GLubyte c = 0; c < 128; c++)
	{
		if (FT_Load_Char(this->face, c, FT_LOAD_RENDER))
		{
			Error::printWarning("Could not load character '" + std::string(1, c) + "'!");
			continue;
		}
		this->characters[c].width = this->face->glyph->bitmap.width;
		this->characters[c].height = this->face->glyph->bitmap.rows;
		this->characters[c].bearingX = this->face->glyph->bitmap_left;
		this->characters[c].bearingY = this->face->glyph->bitmap_top;
		this->characters[c].advance = this->face->glyph->advance.x;

		glBindTexture(GL_TEXTURE_2D, this->characters[c].textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->characters[c].width, this->characters[c].height, 0, GL_RED, GL_UNSIGNED_BYTE, this->face->glyph->bitmap.buffer);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
