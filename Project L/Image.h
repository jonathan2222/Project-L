#ifndef IMAGE_H
#define IMAGE_H

struct Image
{
	int width, height;
	unsigned char* data;
	GLint internalFormat = GL_RGBA;
	GLenum format = GL_RGBA;
	GLenum type = GL_UNSIGNED_BYTE;
};

#endif
