#ifndef TEXTURE_H
#define TEXTURE_H

#include "GL\glew.h"

#include "../Image.h"

class Texture
{
public:
	Texture();
	Texture(const Image* image);
	~Texture();

	GLuint getID() const;

	void bind() const;
	void unbind() const;

	void loadImage(const Image* image);
	void releaseImage();

private:
	GLuint id;
	bool hasImage;
	unsigned int width, height;
};

#endif
