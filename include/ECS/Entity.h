#pragma once

#include <vector>
#include <type_traits>

#include <ECS/Component.h>

class Entity {
public:
	unsigned int id;
	std::vector<Component*> components;

	Component* GetComponentByType(std::string type)
	{
		for (int i = 0; i < components.size(); i++)
		{
			Component* component = components[i];
			if (type == component->GetType())
				return component;
		}

		return nullptr;
	}

	bool HasComponent(std::string type)
	{
		for (int i = 0; i < components.size(); i++)
		{
			Component* component = components[i];
			if (type == component->GetType())
				return true;
		}

		return false;
	}

	bool AddComponent(std::string type)
	{
		if (HasComponent(type))
		{
			return false;
		}

		Component* newComponent = new Component();
		components.push_back(newComponent);
		return true;
	}

	bool RemoveComponent(std::string type)
	{
		if (!HasComponent(type)) 
		{
			return false;
		}

		for (std::vector<Component*>::iterator it = components.begin();
			it != components.end(); it++)
		{
			Component* component = *it;
			if (type == component->GetType())
			{
				components.erase(it);
				return true;
			}
		}
	}
};