#pragma once

#include <vector>

#include <ECS/Entity.h>

class System {
public:
	//System() {};
	//virtual ~System() = 0;

	virtual void Process(const std::vector<Entity*>& entities, float dt) = 0;
};