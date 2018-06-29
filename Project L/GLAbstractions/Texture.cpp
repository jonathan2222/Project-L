#include "Texture.h"

Texture::Texture() : hasImage(false), width(0), height(0)
{
}

Texture::Texture(const Image* image) : hasImage(false)
{
	loadImage(image);
}

Texture::~Texture()
{
	releaseImage();
}

GLuint Texture::getID() const
{
	return this->id;
}

void Texture::bind() const
{
	if(this->hasImage)
		glBindTexture(GL_TEXTURE_2D, this->id);
}

void Texture::unbind() const
{
	if(this->hasImage)
		glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::loadImage(const Image * image)
{
	if (!this->hasImage)
	{
		glGenTextures(1, &this->id);
		this->hasImage = true;
		this->width = image->width;
		this->height = image->height;
		bind();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		unbind();
	}
}

void Texture::releaseImage()
{
	if (this->hasImage)
	{
		glDeleteTextures(1, &this->id);
		this->hasImage = false;
		this->width = 0;
		this->height = 0;
	}
}
