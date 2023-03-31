#pragma once

#include <ECS/System.h>

class MotionSystem : public System {
public:
	MotionSystem();
	~MotionSystem();

	void Process(const std::vector<Entity*>& entities, float dt);
	void Shutdown();

private:
	// name of the system
	std::string systemName;
};