#include "EntityManager.h"

// Constructor
EntityManager::EntityManager()
{
}

// Destructor
EntityManager::~EntityManager()
{
}

// Creates an entity and returns its ID
unsigned int EntityManager::CreateEntity()
{
	Entity* newEntity = new Entity();
	entities.push_back(newEntity);
	return entities.size() - 1;
}

// retruns array of entities
const std::vector<Entity*>& EntityManager::GetEntities()
{
	return entities;
}