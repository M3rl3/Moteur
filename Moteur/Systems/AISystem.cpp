#include "AISystem.h"

#include "../Components/MeshComponent.h"
#include "../Components/PlayerComponent.h"

AISystem::AISystem()
{
	stateMachine = new StateMachine();

	/*stateMachine->AddTransition(IDLE, PURSUE);
	stateMachine->AddTransition(PURSUE, IDLE);
	stateMachine->AddTransition(PURSUE, CATCH);
	stateMachine->AddTransition(CATCH, IDLE);*/
}

AISystem::~AISystem()
{
}

void AISystem::Process(const std::vector<Entity*>& entities, float dt)
{
	AIComponent* aiComponent = nullptr;
	TransformComponent* transformComponent = nullptr;
	TransformComponent* transformComponent_player = nullptr;
	VelocityComponent* velocityComponent = nullptr;
	VelocityComponent* velocityComponent_player = nullptr;
	MeshComponent* meshComponent = nullptr;
	MeshComponent* meshComponent_player = nullptr;

	for (int i = 0; i < entities.size(); i++) {
		
		Entity* currentEntity = entities[i];

		// get the specific instances for all components
		transformComponent = currentEntity->GetComponentByType<TransformComponent>();
		aiComponent = currentEntity->GetComponentByType<AIComponent>();
		velocityComponent = currentEntity->GetComponentByType<VelocityComponent>();
		meshComponent = currentEntity->GetComponentByType<MeshComponent>();

		if (aiComponent != nullptr) {

			// find player component
			transformComponent_player = GetPlayerTransform(entities);
			velocityComponent_player = GetPlayerVelocity(entities);
			meshComponent_player = GetPlayerMesh(entities);

			if (meshComponent_player->health <= 0.f) {
				meshComponent_player->isVisible = false;
			}

			if (meshComponent->health <= 0.f) {
				meshComponent->isVisible = false;
			}

			if (transformComponent != nullptr && velocityComponent != nullptr) {

				glm::vec3 aiPosition = transformComponent->position;
				glm::vec3 playerPosition = transformComponent_player->position;

				glm::vec3 playerFacingDir = velocityComponent_player->facingDirection;

				float distance = GetDistance(aiPosition, playerPosition);

				// pursues/evades x distance ahead of the player
				const float aheadDistance = 25.f;

				glm::vec3 aheadOfPlayer = transformComponent_player->position + playerFacingDir * aheadDistance;

				if (distance < aiComponent->radius) {

					switch (aiComponent->aiBehaviour)
					{
					case BehaviourType::IDLE: {

						velocityComponent->velocity = glm::vec3(0.f);
						break;
					}
					case BehaviourType::SEEK: {

						Face(playerPosition, transformComponent);

						glm::vec3 towardsTarget = glm::normalize(aiPosition - playerPosition);
						glm::vec3 towardsPlayer = -towardsTarget;

						float dotProduct = glm::dot(towardsTarget, playerFacingDir);

						if (dotProduct <= 0.f) {
							velocityComponent->velocity = towardsPlayer * dt;
							velocityComponent->velocity.y = 0.f;
						}
						else {
							velocityComponent->velocity = glm::vec3(0.f);
						}

						if (distance > aiComponent->radius * 2) {
							velocityComponent->velocity = glm::vec3(0.f);
						}

						break;
					}						
					case BehaviourType::FLEE: {

						Face(-playerPosition, transformComponent);

						glm::vec3 towardsTarget = glm::normalize(aiPosition - playerPosition);
						glm::vec3 towardsPlayer = -towardsTarget;

						float dotProduct = glm::dot(towardsTarget, playerFacingDir);

						velocityComponent->velocity = -towardsPlayer * dt;
						velocityComponent->velocity.y = 0;

						if (distance >= aiComponent->radius * 2) {
							velocityComponent->velocity = glm::vec3(0.f);
						}

						break;
					}
					case BehaviourType::PURSUE: {

						Face(playerPosition, transformComponent);

						glm::vec3 towardsTarget = glm::normalize(aiPosition - aheadOfPlayer);
						glm::vec3 towardsPlayer = -towardsTarget;

						float dotProduct = glm::dot(towardsTarget, playerFacingDir);

						if (dotProduct <= 0.f) {
							velocityComponent->velocity = towardsPlayer * dt;
							velocityComponent->velocity.y = 0.f;
						}
						else {
							velocityComponent->velocity = glm::vec3(0.f);
						}
						break;
					}						
					case BehaviourType::EVADE: {

						Face(-playerPosition, transformComponent);

						glm::vec3 towardsTarget = glm::normalize(aiPosition - aheadOfPlayer);
						glm::vec3 towardsPlayer = -towardsTarget;

						velocityComponent->velocity = -towardsPlayer * dt;
						velocityComponent->velocity.y = 0.f;

						if (distance > aiComponent->radius * 2) {
							velocityComponent->velocity = glm::vec3(0.f);
						}
						break;
					}
					case BehaviourType::APPROACH: {

						Face(playerPosition, transformComponent);

						float distance = glm::distance(playerPosition, aiPosition);

						glm::vec3 towardsTarget = glm::normalize(aiPosition - playerPosition);
						glm::vec3 towardsPlayer = -towardsTarget;

						velocityComponent->velocity = towardsPlayer * dt;
						velocityComponent->velocity.y = 0.f;

						// maintains a 0.5 radius distance from the player
						if (distance <= aiComponent->radius * 0.35f + FLT_EPSILON) {
							velocityComponent->velocity = glm::vec3(0.f);

							meshComponent_player->health -= 1;
							meshComponent->health -= 10;
						}
						/*else {
							meshComponent_player->health -= 0.001f;
							meshComponent->health -= 0.01f;
						}*/

						aiComponent->aiVelocity = velocityComponent->velocity;
						break;
					}						
					default:
						// No idea what this behaviour is
						break;
					}
				}
			}
		}
	}
}

void AISystem::Pursue(float dt, float distance, glm::vec3 playerPosition, AIComponent* aiComponent, TransformComponent* transformComponent)
{
	Face(playerPosition, transformComponent);
	float predictTime = distance / aiComponent->speed;
	glm::vec3 predictedTargetPosition = playerPosition * predictTime;
	glm::vec3 position = transformComponent->position;
	glm::vec3 desiredVelocity = glm::normalize(predictedTargetPosition - position) * aiComponent->speed;

	transformComponent->position += desiredVelocity;
}

void AISystem::Shutdown()
{
	return;
}

void AISystem::Face(glm::vec3 targetPosition, TransformComponent* transformComponent)
{
	transformComponent->rotation = 
		glm::quat(glm::lookAt(transformComponent->position, targetPosition, -glm::vec3(0, 1, 0))) *
		glm::quat(glm::vec3(glm::radians(180.f), glm::radians(180.f), 0));

	// Set the Y component to 0
	// transformComponent->rotation *= glm::quat(glm::vec3(1, 0, 1));
}

float AISystem::GetDistance(glm::vec3 aiPosition, glm::vec3 playerPosition)
{
	return glm::distance(aiPosition, playerPosition);
}

TransformComponent* AISystem::GetPlayerTransform(const std::vector<Entity*>& entities)
{
	TransformComponent* transformComponent = nullptr;
	PlayerComponent* playerComponent = nullptr;

	for (Entity* entity : entities) {

		transformComponent = entity->GetComponentByType<TransformComponent>();
		playerComponent = entity->GetComponentByType<PlayerComponent>();

		if (playerComponent != nullptr) {
			return transformComponent;
		}
	}
	return transformComponent;
}

VelocityComponent* AISystem::GetPlayerVelocity(const std::vector<Entity*>& entities)
{
	VelocityComponent* velocityComponent = nullptr;
	PlayerComponent* playerComponent = nullptr;

	for (Entity* entity : entities) {

		velocityComponent = entity->GetComponentByType<VelocityComponent>();
		playerComponent = entity->GetComponentByType<PlayerComponent>();

		if (playerComponent != nullptr) {
			return velocityComponent;
		}
	}

	return nullptr;
}

MeshComponent* AISystem::GetPlayerMesh(const std::vector<Entity*>& entities)
{
	MeshComponent* meshComponent = nullptr;
	PlayerComponent* playerComponent = nullptr;

	for (Entity* entity : entities) {

		meshComponent = entity->GetComponentByType<MeshComponent>();
		playerComponent = entity->GetComponentByType<PlayerComponent>();

		if (playerComponent != nullptr) {
			return meshComponent;
		}
	}

	return nullptr;
}

IdleState::IdleState()
{
}

IdleState::~IdleState()
{
}

void IdleState::Enter(Entity* entity)
{
	transformComponent = entity->GetComponentByType<TransformComponent>();
	aiComponent = entity->GetComponentByType<AIComponent>();
	velocityComponent = entity->GetComponentByType<VelocityComponent>();
	meshComponent = entity->GetComponentByType<MeshComponent>();

	aiComponent->aiBehaviour = BehaviourType::IDLE;
}

void IdleState::Update(Entity* entity)
{
}

void IdleState::Exit(Entity* entity)
{
}

PursueState::PursueState()
{
}

PursueState::~PursueState()
{
}

void PursueState::Enter(Entity* entity)
{
	transformComponent = entity->GetComponentByType<TransformComponent>();
	aiComponent = entity->GetComponentByType<AIComponent>();
	velocityComponent = entity->GetComponentByType<VelocityComponent>();
	meshComponent = entity->GetComponentByType<MeshComponent>();

	aiComponent->aiBehaviour = BehaviourType::PURSUE;
}

void PursueState::Update(Entity* entity)
{
}

void PursueState::Exit(Entity* entity)
{
}

// Catch state
CatchState::CatchState()
{
}

CatchState::~CatchState()
{
}

void CatchState::Enter(Entity* entity)
{
	transformComponent = entity->GetComponentByType<TransformComponent>();
	aiComponent = entity->GetComponentByType<AIComponent>();
	velocityComponent = entity->GetComponentByType<VelocityComponent>();
	meshComponent = entity->GetComponentByType<MeshComponent>();

	aiComponent->aiBehaviour = BehaviourType::CATCH;
}

void CatchState::Update(Entity* entity)
{
}

void CatchState::Exit(Entity* entity)
{
}

// State Machine
StateMachine::StateMachine()
	: m_CurrentState(nullptr)
{
	m_CurrentState = new IdleState();
}

StateMachine::~StateMachine()
{
}

void StateMachine::AddTransition(BehaviourType from, BehaviourType to)
{
	std::vector<BehaviourType>& stateVec = m_ValidTransitions[from];
	if (std::find(stateVec.begin(), stateVec.end(), to) == stateVec.end())
	{
		stateVec.push_back(to);
	}
}

State* StateMachine::GetCurrentState()
{
	if (m_CurrentState != nullptr) {
		return m_CurrentState;
	}
	else return nullptr;
}

//void StateMachine::SetState(State* state)
//{
//	if (m_CurrentState != nullptr) {
//
//		if (m_CurrentState->GetType() == state->GetType())
//		{
//			return;
//		}
//
//		// m_CurrentState->Exit();
//	}	
//
//	std::vector<BehaviourType>& stateVec = m_ValidTransitions[m_CurrentState->GetType()];
//	if (std::find(stateVec.begin(), stateVec.end(), state->GetType()) == stateVec.end())
//	{
//		printf("!! Not a valid transition!\n");
//		// No valid transition was added from the current state to the new state
//		return;
//	}
//
//	// m_CurrentState->Exit();
//	delete m_CurrentState;
//
//	m_CurrentState = state;
//	// m_CurrentState->Enter();
//}