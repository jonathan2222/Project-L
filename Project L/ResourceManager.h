#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <unordered_map>
#include <string>

#include "Utils\STB\stb_image.h"
#include "Utils\Error.h"
#include "GLAbstractions\Texture.h"

#include "GameConfig.h"

class ResourceManager
{
public:
	static void initResources(const std::string& pathToDefaultImage)
	{
		Image* img = loadImage(pathToDefaultImage);
		if (img == nullptr)
		{
			Error::printWarning("ResourceManager::initResources()", "Default image could not be loaded! Path: '" + pathToDefaultImage + "'");
			return;
		}
		defaultTexture = createTexture(img);
	}

	static void freeResources()
	{
		for (std::unordered_map<std::string, Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
			delete it->second;
		textures.clear();
		delete defaultTexture;
	}

	/*
	Add a texture to the resource manager's storage.
	Arguments:
		name: A name given to this resrouce.
		path: A path with resource name eg. ./foo/bar.png

	Return nullptr if no image was found and if no default image was found.
	*/
	static Texture* addTexture(const std::string& name, const std::string& path)
	{
		std::unordered_map<std::string, Texture*>::iterator it = textures.find(name);
		if (it != textures.end()) // Already added.
		{
			Error::printWarning("ResourceManager::addTexture()", "Texture already loaded! Name: '" + name + "'");
			return nullptr;
		}

		Image* img = loadImage(path);
		if (img == nullptr)
			return nullptr;

		Texture* texture = createTexture(img);
		textures[name] = texture;
		return texture;
	}

	/*
	Remove a texture from the storage.
	Returns ture if removed else false. 
	*/
	static bool removeTexture(const std::string& name)
	{
		std::unordered_map<std::string, Texture*>::iterator it = textures.find(name);
		if (it != textures.end())
		{
			delete it->second;
			textures.erase(name);
			return true;
		}
		return false; // Texture was not found.
	}

	/*
	Get a texture which holds the width and height and the pixel data.
	Return nullptr if no texture was found and if no default texture was found.
	*/
	static Texture* getTexture(const std::string& name)
	{
		std::unordered_map<std::string, Texture*>::iterator it = textures.find(name);
		if (it != textures.end()) // Found texture
			return it->second;

		if (defaultTexture == nullptr) // No default texture.
			return nullptr;

		return defaultTexture;
	}

	/*
	Return nullptr if no default texture was found.
	*/
	static const Texture* getDefaultTexture()
	{
		if (defaultTexture == nullptr)
			return nullptr;
		return defaultTexture;
	}

private:
	/*
	Will create a texture from image and delete the image.
	*/
	static Texture* createTexture(Image* img)
	{
		Texture* texture = new Texture(img);
		stbi_image_free(img->data);
		delete img;
		return texture;
	}

	static Image* loadImage(const std::string& path)
	{
		Image* img = new Image();
		int channelsInternal;
		img->data = stbi_load((std::string(PATH_TO_RESOURCES) + path).c_str(), &img->width, &img->height, &channelsInternal, STBI_rgb_alpha);

		if (img->data == nullptr) // Could not load image.
		{
			Error::printWarning("ResourceManager::loadImage()", "Could not load image! Path: '" + path + "'");
			delete img;
			return nullptr;
		}

		return img;
	}

	static std::unordered_map<std::string, Texture*> textures;
	static Texture* defaultTexture;
};

#endif
