#pragma once

#include <vector>

#include <ECS/Entity.h>

// Interface for all systems 
class System {
public:

	// Constructor
	System() {}
	// Pure virtual Destructor
	virtual ~System() {}

	// Pure virtual methods
	// The update method that will be implemented by all systems
	virtual void Process(const std::vector<Entity*>& entities, float dt) = 0;

	// Shutdown method for all systems
	virtual void Shutdown() = 0;

private:
	// name of the system
	std::string systemName;
};