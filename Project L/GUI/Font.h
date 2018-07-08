#ifndef FONT_H
#define FONT_H

#include <string>

#include "Display.h"
#include "../Shaders/Shader.h"
#include <unordered_map>
#include "../GLAbstractions/Texture.h"

class Font
{
public:
	struct Character
	{
		GLuint textureID;
		int width;		// Glyph size in X.
		int height;		// Glyph size in Y.
		int bearingX;	// Distance from origon to left side of glyph.
		int bearingY;	// Distance from origon to top side of glyph.
		GLuint advance;	// How far to jump.
	};
public:
	Font();
	Font(const std::string& fontPath, unsigned int size, Display* display);
	~Font();

	bool load(const std::string& fontPath, unsigned int size, Display* display);

	// Note: This is slow
	void setSize(unsigned int size);

	Shader* getShader();
	Character& getCharacter(char c);
	std::unordered_map<char, Character>& getCharacters();

	static void FREE_SHADER();

private:
	void freeCharacters();
	void loadCharacters();
	void updateCharacters();

	static Shader * shader;
	FT_Face face;

	std::unordered_map<char, Character> characters;
};

#endif
