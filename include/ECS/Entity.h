#pragma once

#include <vector>
#include <type_traits>

#include <ECS/Component.h>

class Entity {
public:
	unsigned int id;
	std::vector<Component*> components;

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

	template <class T>
	T* AddComponent()
	{
		static_assert(std::is_base_of<Component, T>::value);

		if (HasComponent<T>())
		{
			return;
		}

		T* newComponent = new T();
		components.push_back(newComponent);
		return newComponent;
	}

	template <class T>
	T* RemoveComponent()
	{
		for (std::vector<Component*>::iterator it = components.begin();
			it != components.end(); it++)
		{
			T* component = dynamic_cast<T*>(*it);
			if (component != nullptr)
			{
				components.erase(it);
				return *it;
			}
		}

		return nullptr;
	}
};