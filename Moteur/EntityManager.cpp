#include "EntityManager.h"

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
}

unsigned int EntityManager::CreateEntity()
{
	Entity* newEntity = new Entity();
	entities.push_back(newEntity);
	return entities.size() - 1;
}

Component* EntityManager::AddComponent(unsigned int entityId, std::string componentType)
{
	assert(entityId < entities.size());
	Entity* entity = entities[entityId];
	return entity->AddComponent(componentType);
}

void EntityManager::RemoveComponent(unsigned int entityId, std::string componentType)
{
	assert(entityId < entities.size());
	Entity* entity = entities[entityId];
	entity->RemoveComponent(componentType);
}

const std::vector<Entity*>& EntityManager::GetEntities()
{
	return entities;
}