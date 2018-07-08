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

	void setImage(const Image* image, GLint magFilter = GL_NEAREST, GLint minFilter = GL_NEAREST, GLint warpS = GL_REPEAT, GLint warpT = GL_REPEAT);

	GLuint getID() const;

	void bind() const;
	void unbind() const;

	void releaseImage();

private:
	void init();

	GLuint id;
	unsigned int width, height;
};

#endif
