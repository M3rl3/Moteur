#pragma once

#include <vector>

#include <ECS/Entity.h>

class System {
public:

	System() {}
	virtual ~System() {}

	virtual void Process(const std::vector<Entity*>& entities, float dt) = 0;
	virtual void Shutdown() = 0;

private:
	std::string systemName;
};