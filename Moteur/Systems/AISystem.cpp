#include "AISystem.h"

#include "../Components/MeshComponent.h"
#include "../Components/PlayerComponent.h"

#include <typeinfo>

int frameCount = 0;

AISystem::AISystem()
{
	stateMachine = new StateMachine();
	statePool = new StatePool(9);

	idleState = new IdleState();
	pursueState = new PursueState();
	catchState = new CatchState();

	stateMachine->AddTransition(IDLE, PURSUE);
	stateMachine->AddTransition(PURSUE, IDLE);
	stateMachine->AddTransition(PURSUE, CATCH);
	stateMachine->AddTransition(CATCH, IDLE);
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

	Entity* playerEntity = nullptr;
	PlayerComponent* playerComponent = nullptr;

	for (int i = 0; i < entities.size(); i++) {
		
		Entity* currentEntity = entities[i];

		// get the specific instances for all components
		transformComponent = currentEntity->GetComponentByType<TransformComponent>();
		aiComponent = currentEntity->GetComponentByType<AIComponent>();
		velocityComponent = currentEntity->GetComponentByType<VelocityComponent>();
		meshComponent = currentEntity->GetComponentByType<MeshComponent>();

		if (aiComponent != nullptr) {

			// find player entity
			playerEntity = GetPlayerEntity(entities);

			// find player components
			transformComponent_player = GetPlayerTransform(entities);
			velocityComponent_player = GetPlayerVelocity(entities);
			meshComponent_player = GetPlayerMesh(entities);
			playerComponent = GetPlayerComponent(entities);

			if (playerComponent->health <= 0.f) {
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

				if (aiComponent->doOnce) {
					aiComponent->doOnce = false;
				}
				
				if (distance >= aiComponent->radius) {
					aiComponent->currentState = statePool->GetState<IdleState>();
				}
				else {
					aiComponent->currentState = statePool->GetState<PursueState>();
				}

				aiComponent->currentState->Enter(currentEntity);
				aiComponent->currentState->Update(dt, playerEntity);

				statePool->ReturnState(aiComponent->currentState);
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

Entity* AISystem::GetPlayerEntity(const std::vector<Entity*>& entities)
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

PlayerComponent* AISystem::GetPlayerComponent(const std::vector<Entity*>& entities)
{
	PlayerComponent* playerComponent = nullptr;

	for (Entity* entity : entities) {

		playerComponent = entity->GetComponentByType<PlayerComponent>();

		if (playerComponent != nullptr) {
			return playerComponent;
		}
	}

	return nullptr;
}

IdleState::IdleState()
{
	typeName = "IdleState";
}

IdleState::~IdleState()
{
}

const char* IdleState::GetType() const
{
	return typeid(IdleState).name();
}

void IdleState::Enter(Entity* entity)
{
	transformComponent = entity->GetComponentByType<TransformComponent>();
	aiComponent = entity->GetComponentByType<AIComponent>();
	velocityComponent = entity->GetComponentByType<VelocityComponent>();
	meshComponent = entity->GetComponentByType<MeshComponent>();

	aiComponent->aiBehaviour = BehaviourType::IDLE;
	aiEntity = entity;
}

void IdleState::Update(float dt, Entity* playerEntity)
{
	velocityComponent->velocity = glm::vec3(0.f);
}

void IdleState::Exit()
{
	aiComponent->aiBehaviour = BehaviourType::IDLE;
	aiComponent->currentState = nullptr;
}

PursueState::PursueState()
{
	typeName = "PursueState";
}

PursueState::~PursueState()
{
}

const char* PursueState::GetType() const
{
	return typeid(PursueState).name();
}

void PursueState::Enter(Entity* entity)
{
	transformComponent = entity->GetComponentByType<TransformComponent>();
	aiComponent = entity->GetComponentByType<AIComponent>();
	velocityComponent = entity->GetComponentByType<VelocityComponent>();
	meshComponent = entity->GetComponentByType<MeshComponent>();

	aiComponent->aiBehaviour = BehaviourType::PURSUE;
	aiEntity = entity;
}

void PursueState::Update(float dt, Entity* playerEntity)
{
	TransformComponent* transformComponent_player = nullptr;
	VelocityComponent* velocityComponent_player = nullptr;

	transformComponent_player = playerEntity->GetComponentByType<TransformComponent>();
	velocityComponent_player = playerEntity->GetComponentByType<VelocityComponent>();

	glm::vec3 playerPosition = transformComponent_player->position;
	glm::vec3 aiPosition = transformComponent->position;

	glm::vec3 playerVelocity = velocityComponent_player->velocity;
	glm::vec3 aiVelocity = velocityComponent->velocity;

	glm::vec3 playerFacingDirection = velocityComponent_player->facingDirection;

	// pursues/evades x distance ahead of the player
	const float aheadDistance = 25.f;
	glm::vec3 aheadOfPlayer = playerPosition + playerFacingDirection * aheadDistance;

	float distance = glm::distance(aiPosition, playerPosition);

	if (distance < aiComponent->radius) {

		// Face
		transformComponent->rotation =
			glm::quat(glm::lookAt(aiPosition, playerPosition, -glm::vec3(0, 1, 0))) *
			glm::quat(glm::vec3(glm::radians(180.f), glm::radians(180.f), 0));

		// Pursue
		glm::vec3 towardsTarget = glm::normalize(aiPosition - aheadOfPlayer);
		glm::vec3 towardsPlayer = -towardsTarget;

		float dotProduct = glm::dot(towardsTarget, playerFacingDirection);

		if (dotProduct <= 0.f) {
			velocityComponent->velocity = towardsPlayer * dt;
			velocityComponent->velocity.y = 0.f;
		}
		else {
			velocityComponent->velocity = glm::vec3(0.f);
		}
	}	
}

void PursueState::Exit()
{
	aiComponent->aiBehaviour = BehaviourType::IDLE;
	aiComponent->currentState = nullptr;
}

// Catch state
CatchState::CatchState()
{
	typeName = "CatchState";
}

CatchState::~CatchState()
{
}

const char* CatchState::GetType() const
{
	return typeid(CatchState).name();
}

void CatchState::Enter(Entity* entity)
{
	transformComponent = entity->GetComponentByType<TransformComponent>();
	aiComponent = entity->GetComponentByType<AIComponent>();
	velocityComponent = entity->GetComponentByType<VelocityComponent>();
	meshComponent = entity->GetComponentByType<MeshComponent>();

	aiComponent->aiBehaviour = BehaviourType::CATCH;
	aiEntity = entity;
}

void CatchState::Update(float dt, Entity* playerEntity)
{
	frameCount++;

	TransformComponent* transformComponent_player = nullptr;
	VelocityComponent* velocityComponent_player = nullptr;
	PlayerComponent* playerComponent = nullptr;

	transformComponent_player = playerEntity->GetComponentByType<TransformComponent>();
	velocityComponent_player = playerEntity->GetComponentByType<VelocityComponent>();
	playerComponent = playerEntity->GetComponentByType<PlayerComponent>();

	float catchDuration = 5.f; // 5 seconds
	float catchTimer = catchDuration * 60.f;

	if (playerComponent != nullptr) {
		if (frameCount >= 300) {
			playerComponent->isControllable = false;
		}
		else {
			playerComponent->isControllable = true;
		}
	}
}

void CatchState::Exit()
{
	aiComponent->aiBehaviour = BehaviourType::IDLE;
	aiComponent->currentState = nullptr;
}

// State Machine
StateMachine::StateMachine() :
	m_CurrentState(nullptr),
	entity(nullptr),
	catchTimer(0)
{
	m_CurrentState = new IdleState();
}

StateMachine::~StateMachine()
{
	m_CurrentState = nullptr;
	delete m_CurrentState;
}

void StateMachine::AddTransition(BehaviourType from, BehaviourType to)
{
	std::vector<BehaviourType>& stateVec = m_ValidTransitions[from];
	if (std::find(stateVec.begin(), stateVec.end(), to) == stateVec.end())
	{
		stateVec.push_back(to);
	}
}

void StateMachine::Update(float dt, Entity* playerEntity)
{
	if (m_CurrentState) {
		m_CurrentState->Update(dt, playerEntity);

		/*if (m_CurrentState->GetType() == BehaviourType::CATCH) {
			catchTimer -= 1;

			if (catchTimer <= 0) {
				m_CurrentState = nullptr;
				delete m_CurrentState;

				m_CurrentState = new IdleState();
			}
		}*/
	}
}

State* StateMachine::GetCurrentState()
{
	if (m_CurrentState != nullptr) {
		return m_CurrentState;
	}
	else return nullptr;
}

void StateMachine::SetState(State* newState, Entity* entity)
{
	if (m_CurrentState != nullptr) {

		if (m_CurrentState->GetType() == newState->GetType())
		{
			return;
		}

		m_CurrentState->Exit();
	}	

	m_CurrentState = newState;

	aiComponent = entity->GetComponentByType<AIComponent>();
	aiComponent->currentState = m_CurrentState;

	// TODO: add valid transitions
	
	//std::vector<BehaviourType>& stateVec = m_ValidTransitions[m_CurrentState->GetType()];
	//if (std::find(stateVec.begin(), stateVec.end(), newState->GetType()) == stateVec.end())
	//{
	//	printf("!! Not a valid transition!\n");
	//	// No valid transition was added from the current state to the new state
	//	return;
	//}

	m_CurrentState->Enter(entity);
}

void StateMachine::SetCatchTimer(int timer)
{
	catchTimer = timer;
}

StatePool::StatePool(int size)
{
	int newSize = size;

	if (newSize % 2 != 0) {
		newSize++;
	}
	for (int i = 0; i < newSize * 0.5f; i++) {
		pool.push_back(new IdleState());
	}
	for (int i = 0; i < newSize * 0.5f; i++) {
		pool.push_back(new PursueState());
	}
	for (int i = 0; i < newSize * 0.5f; i++) {
		pool.push_back(new CatchState());
	}
	int breakPoint = 0;
}

StatePool::~StatePool()
{
	for (auto state : pool) {
		delete state;
	}
}

template<class T>
T* StatePool::GetState()
{
	for (auto it = pool.begin(); it != pool.end(); it++) {

		if ((*it)->GetType() == typeid(T).name()) {

			T* state = dynamic_cast<T*>(*it);
			pool.erase(it);
			return state;
		}
	}

	T* newState = new T();
	pool.push_back(newState);
	return newState;
}

State* StatePool::GetState()
{
	if (pool.empty()) {
		pool.push_back(new IdleState());
	}
	State* state = pool.back();
	pool.pop_back();
	return state;
}

void StatePool::ReturnState(State* state)
{
	pool.push_back(state);
}

std::vector<State*>& StatePool::GetStatePool()
{
	return pool;
}