#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H

#include <unordered_map>

#include "Model.h"
#include "../Utils/Error.h"

class ModelManager
{
public:
	static Model* get(const std::string& name)
	{
		std::unordered_map<std::string, Model*>::iterator it = models.find(name);
		if (it != models.end()) // Found model
			return it->second;
		return nullptr;
	}

	static bool add(const std::string& name, Model* model)
	{
		std::unordered_map<std::string, Model*>::iterator it = models.find(name);
		if (it != models.end()) // Already added.
			return false;
		models[name] = model;
		return true;
	}

	static bool remove(const std::string& name)
	{
		std::unordered_map<std::string, Model*>::iterator it = models.find(name);
		if (it != models.end()) // Model found.
		{
			delete it->second;
			return true;
		}
		return false;
	}

	static void removeAll()
	{
		for (std::unordered_map<std::string, Model*>::iterator it = models.begin(); it != models.end(); it++)
			delete it->second;
		models.clear();
	}

private:
	static std::unordered_map<std::string, Model*> models;
};

#endif
