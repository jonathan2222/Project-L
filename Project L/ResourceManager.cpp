#include "ResourceManager.h"

std::unordered_map<std::string, Texture*> ResourceManager::textures = std::unordered_map<std::string, Texture*>();
Texture* ResourceManager::defaultTexture = nullptr;