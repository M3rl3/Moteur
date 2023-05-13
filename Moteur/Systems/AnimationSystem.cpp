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

	// Get all necessary components
	animationComponent = playerEntity->GetComponentByType<AnimationComponent>();
	velocityComponent = playerEntity->GetComponentByType<VelocityComponent>();

	// Check if the components actually exist
	if (animationComponent != nullptr && velocityComponent != nullptr) {

		// Get the current speed
		float speed = glm::length(velocityComponent->velocity);

		// Set the state
		if (speed != 0.f && animationComponent->sprint) {
			animationComponent->animationState = AnimationState::SPRINT;
		}
		else if (speed != 0.f) {
			animationComponent->animationState = AnimationState::WALK;
		}
		else {
			animationComponent->animationState = AnimationState::BREATHING_IDLE;
		}

		// Switch between animation states
		switch (animationComponent->animationState)
		{
		case AnimationState::BREATHING_IDLE: {
			animationComponent->animator->PlayAnimation(animationComponent->animation[0], false);
			break;
		}
		case AnimationState::WALK: {
			velocityComponent->velocity *= 0.6f;
			animationComponent->animator->PlayAnimation(animationComponent->animation[1], false);
			break;
		}
		case AnimationState::SPRINT: {
			animationComponent->animator->PlayAnimation(animationComponent->animation[2], false);
			break;
		}
		default:
			// No idea what state this is
			break;
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
