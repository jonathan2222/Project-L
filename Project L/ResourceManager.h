#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <unordered_map>
#include <string>

#include "Utils\STB\stb_image.h"
#include "Utils\Error.h"

#include "GameConfig.h"

class ResourceManager
{
public:
	struct Image
	{
		int width, height;
		unsigned char* data;
	};
public:
	static void initResources(const std::string& pathToDefaultImage)
	{
		defaultImage = loadImage(std::string(PATH_TO_RESOURCES) + pathToDefaultImage);
		if (defaultImage == nullptr)
		{
			Error::printWarning("ResourceManager::initResources()", "Default image could not be loaded! Path: '" + std::string(PATH_TO_RESOURCES) + pathToDefaultImage + "'");
			return;
		}
	}

	static void freeResources()
	{
		for (std::unordered_map<std::string, ResourceManager::Image*>::iterator it = images.begin(); it != images.end(); ++it)
		{
			stbi_image_free(it->second->data);
			delete it->second;
		}
		images.clear();
		stbi_image_free(defaultImage->data);
		delete defaultImage;
	}

	/*
	Add an image to the resource manager's storage.
	Arguments:
		name: A name given to this resrouce.
		path: A path with resource name eg. ./foo/bar.png

	Return nullptr if no image was found and if no default image was found.
	*/
	static const Image* addImage(const std::string& name, const std::string& path)
	{
		std::unordered_map<std::string, ResourceManager::Image*>::iterator it = images.find(name);
		if (it != images.end()) // Already added.
		{
			Error::printWarning("ResourceManager::addImage()", "Image already loaded! Name: '" + name + "'");
			return nullptr;
		}

		Image* img = loadImage(path);
		if (img == nullptr)
			return nullptr;

		images[name] = img;
		return img;
	}

	/*
	Get an Image which holds the width and height and the pixel data.
	Return nullptr if no image was found and if no default image was found.
	*/
	static const Image* getImage(const std::string& name)
	{
		std::unordered_map<std::string, ResourceManager::Image*>::iterator it = images.find(name);
		if (it != images.end()) // Found image
			return it->second;

		if (defaultImage == nullptr) // No default image.
			return nullptr;

		return defaultImage;
	}

	/*
	Return nullptr if no default image was found.
	*/
	static const Image* getDefaultImage()
	{
		if (defaultImage == nullptr)
			return nullptr;
		return defaultImage;
	}

private:
	static Image* loadImage(const std::string& path)
	{
		Image* img = new Image();
		int channelsInternal;
		img->data = stbi_load(path.c_str(), &img->width, &img->height, &channelsInternal, STBI_rgb_alpha);

		if (img->data == nullptr) // Could not load image.
		{
			Error::printWarning("ResourceManager::loadImage()", "Could not load image! Path: '" + path + "'");
			delete img;
			return nullptr;
		}

		return img;
	}

	static std::unordered_map<std::string, Image*> images;
	static Image* defaultImage;
};

#endif
