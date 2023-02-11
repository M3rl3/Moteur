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

	Component* AddComponent(unsigned int entityId, std::string componentType);

	void RemoveComponent(unsigned int entityId, std::string componentType);

	const std::vector<Entity*>& GetEntities();

private:
	std::vector<Entity*> entities;
	std::map<unsigned int, std::vector<Component*>> entityTypes;
};