#include "Texture.h"

Texture::Texture(const ResourceManager::Image* image)
{
	glGenTextures(1, &this->id);

	if (image == nullptr)
		this->hasImage = false;
	else
	{
		this->hasImage = true;
		loadImage(image);
	}

	bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture::~Texture()
{
	glDeleteTextures(1, &this->id);
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

void Texture::loadImage(const ResourceManager::Image * image) const
{
	if (!this->hasImage)
	{
		bind();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		unbind();
	}
}
