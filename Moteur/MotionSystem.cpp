#include "MotionSystem.h"

#include "VelocityComponent.h"
#include "TransformComponent.h"

MotionSystem::MotionSystem()
{
}

MotionSystem::~MotionSystem()
{
}

void MotionSystem::Process(const std::vector<Entity*>& entities, float dt) {

	TransformComponent* transformComponent = nullptr;
	VelocityCompoent* velocityComponent = nullptr;

	for (int i = 0; i < entities.size(); i++) {
		transformComponent = entities[i]->GetComponentByType<TransformComponent>();
		velocityComponent = entities[i]->GetComponentByType<VelocityCompoent>();

		if (transformComponent != nullptr && velocityComponent != nullptr) {
			if (velocityComponent->targeting) {
				if (velocityComponent->destination != transformComponent->position) {
					velocityComponent->destination -= transformComponent->position;
					velocityComponent->velocity = glm::normalize(velocityComponent->destination);

					transformComponent->position += velocityComponent->velocity * dt;
				}
				else {
					velocityComponent->velocity = glm::vec3(0.f);
				}
			}
			else {
				velocityComponent->velocity = glm::normalize(velocityComponent->velocity);
				transformComponent->position += velocityComponent->velocity * dt;
			}
		}
	}
}

void MotionSystem::Shutdown() {
	return;
}