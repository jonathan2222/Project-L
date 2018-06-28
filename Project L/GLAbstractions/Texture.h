#ifndef TEXTURE_H
#define TEXTURE_H

#include "GL\glew.h"

#include "../ResourceManager.h"

class Texture
{
public:
	Texture();
	Texture(const ResourceManager::Image* image = nullptr);
	~Texture();

	GLuint getID() const;

	void bind() const;
	void unbind() const;

	void loadImage(const ResourceManager::Image* image);
	void releaseImage();

private:
	GLuint id;
	bool hasImage;
};

#endif
