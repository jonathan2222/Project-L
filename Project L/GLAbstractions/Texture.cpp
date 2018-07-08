#include "Texture.h"

Texture::Texture() : width(0), height(0)
{
	init();
}

Texture::Texture(const Image* image)
{
	init();
	setImage(image);
}

Texture::~Texture()
{
	releaseImage();
}

void Texture::setImage(const Image * image, GLint magFilter, GLint minFilter, GLint warpS, GLint warpT)
{
	this->width = image->width;
	this->height = image->height;
	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, image->internalFormat, image->width, image->height, 0, image->format, image->type, image->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, warpS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, warpT);
	unbind();
}

GLuint Texture::getID() const
{
	return this->id;
}

void Texture::bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->id);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::releaseImage()
{
	glDeleteTextures(1, &this->id);
	this->width = 0;
	this->height = 0;
}

void Texture::init()
{
	glGenTextures(1, &this->id);
}
