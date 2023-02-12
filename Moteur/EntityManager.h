#pragma once

#include <ECS/Entity.h>
#include <assert.h>

#include <vector>
#include <map>

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	unsigned int CreateEntity();

	template <class T>
	T* AddComponent(unsigned int entityId)
	{
		assert(entityId < entities.size());
		Entity* entity = entities[entityId];
		return entity->AddComponent<T>();
	}

	template <class T>
	bool RemoveComponent(unsigned int entityId)
	{
		assert(entityId < entities.size());
		Entity* entity = entities[entityId];
		return entity->RemoveComponent<T>();
	}

	const std::vector<Entity*>& GetEntities();

private:
	std::vector<Entity*> entities;
	std::map<unsigned int, std::vector<Component*>> entityTypes;
};