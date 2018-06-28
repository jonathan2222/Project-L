#include "ResourceManager.h"

std::unordered_map<std::string, ResourceManager::Image*> ResourceManager::images = std::unordered_map<std::string, ResourceManager::Image*>();
ResourceManager::Image* ResourceManager::defaultImage = nullptr;