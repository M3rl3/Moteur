#include "AnimationSystem.h"

#include "../Components/AnimationComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/PlayerComponent.h"

AnimationSystem::AnimationSystem()
{
}

AnimationSystem::~AnimationSystem()
{
}

void AnimationSystem::Process(const std::vector<Entity*>& entities, float dt)
{
	AnimationComponent* animationComponent = nullptr;
	VelocityComponent* velocityComponent = nullptr;

	Entity* playerEntity = GetPlayerEntity(entities);

	animationComponent = playerEntity->GetComponentByType<AnimationComponent>();
	velocityComponent = playerEntity->GetComponentByType<VelocityComponent>();

	if (animationComponent != nullptr && velocityComponent != nullptr) {

		float speed = glm::length(velocityComponent->velocity);

		if (speed != 0.f && animationComponent->sprint) {
			animationComponent->animator->PlayAnimation(animationComponent->animation[2], false);
		}
		else if (speed != 0.f) {
			velocityComponent->velocity *= 0.6f;
			animationComponent->animator->PlayAnimation(animationComponent->animation[1], false);
		}
		else {
			animationComponent->animator->PlayAnimation(animationComponent->animation[0], false);
		}
	}
}

void AnimationSystem::Shutdown()
{
}

Entity* AnimationSystem::GetPlayerEntity(const std::vector<Entity*>& entities)
{
	PlayerComponent* playerComponent = nullptr;

	for (Entity* entity : entities) {

		playerComponent = entity->GetComponentByType<PlayerComponent>();

		if (playerComponent != nullptr) {
			return entity;
		}
	}

	return nullptr;
}
