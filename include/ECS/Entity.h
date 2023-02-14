#pragma once

#include <vector>
#include <type_traits>

#include <ECS/Component.h>

// Interface for all entities
class Entity {
public:
	// array to hold all components that will be added
	std::vector<Component*> components;

	// returns the specific instance of the component
	template <class T>
	T* GetComponentByType()
	{
		for (int i = 0; i < components.size(); i++)
		{
			T* component = dynamic_cast<T*>(components[i]);
			if (component != nullptr)
				return component;
		}

		return nullptr;
	}

	// Checks if a component is already added
	template <class T>
	bool HasComponent()
	{
		for (int i = 0; i < components.size(); i++)
		{
			T* component = dynamic_cast<T*>(components[i]);
			if (component != nullptr)
				return true;
		}

		return false;
	}

	// Adds a component and calls constructor on it
	template <class T>
	T* AddComponent()
	{
		if (HasComponent<T>())
		{
			return nullptr;
		}

		T* newComponent = new T();
		components.push_back(newComponent);
		return newComponent;
	}

	// Removes a components and erases it
	template <class T>
	bool RemoveComponent()
	{
		for (std::vector<Component*>::iterator it = components.begin();
			it != components.end(); it++)
		{
			T* component = dynamic_cast<T*>(*it);
			if (component != nullptr)
			{
				components.erase(it);
				return true;
			}
		}
		return false;
	}

	// Returns all the components added to the entity
	std::vector<Component*>& GetComponents() {
		return components;
	}

private:
	// identifier for each entity
	unsigned int id;
};