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

const std::vector<Entity*>& EntityManager::GetEntities()
{
	return entities;
}