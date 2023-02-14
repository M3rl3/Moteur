#pragma once

#include <ECS/Entity.h>
#include <assert.h>

#include <vector>
#include <map>

// Manager for all added entities
class EntityManager
{
public:
	// Constructor
	EntityManager();

	// Destructor
	~EntityManager();

	// Creates an entity and returns its ID
	unsigned int CreateEntity();

	// Add a component that inherits from class Component
	template <class T>
	T* AddComponent(unsigned int entityId)
	{
		assert(entityId < entities.size());
		Entity* entity = entities[entityId];
		return entity->AddComponent<T>();
	}

	// Remove a component
	template <class T>
	bool RemoveComponent(unsigned int entityId)
	{
		assert(entityId < entities.size());
		Entity* entity = entities[entityId];
		return entity->RemoveComponent<T>();
	}

	// retruns array of entities
	const std::vector<Entity*>& GetEntities();

private:
	// array consisting of all entities added
	std::vector<Entity*> entities;

	// map for all entity types	
	std::map<unsigned int, std::vector<Component*>> entityTypes;
};