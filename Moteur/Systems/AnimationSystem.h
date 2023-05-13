#pragma once

#include <ECS/System.h>

class AnimationSystem : public System {
public:
	AnimationSystem();
	~AnimationSystem();

	void Process(const std::vector<Entity*>& entities, float dt);
	void Shutdown();

	Entity* GetPlayerEntity(const std::vector<Entity*>& entities);

private:
	// name of the system
	std::string systemName;
};