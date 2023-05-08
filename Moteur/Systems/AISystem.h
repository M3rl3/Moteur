#pragma once

#include <glm/glm.hpp>
#include <ECS/System.h>

#include "../Components/AIComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/MeshComponent.h"

class StatePool;

class IdleState : public State
{
public:
	IdleState();
	virtual ~IdleState();

	virtual void Enter(Entity* entity) override;
	virtual void Update(float dt, Entity* playerEntity) override;
	virtual void Exit() override;
	virtual const char* C_Str() override { return "IdleState"; }

private:
	AIComponent* aiComponent = nullptr;
	TransformComponent* transformComponent = nullptr;
	VelocityComponent* velocityComponent = nullptr;
	MeshComponent* meshComponent = nullptr;

	Entity* playerEntity = nullptr;
	Entity* aiEntity = nullptr;
};

class PursueState : public State
{
public:
	PursueState();
	virtual ~PursueState();

	virtual void Enter(Entity* entity) override;
	virtual void Update(float dt, Entity* playerEntity) override;
	virtual void Exit() override;
	virtual const char* C_Str() override { return "PursueState"; }

private:
	AIComponent* aiComponent = nullptr;
	TransformComponent* transformComponent = nullptr;
	VelocityComponent* velocityComponent = nullptr;
	MeshComponent* meshComponent = nullptr;

	Entity* playerEntity = nullptr;
	Entity* aiEntity = nullptr;
};

class CatchState : public State
{
public:
	CatchState();
	virtual ~CatchState();

	virtual void Enter(Entity* entity) override;
	virtual void Update(float dt, Entity* playerEntity) override;
	virtual void Exit() override;
	virtual const char* C_Str() override { return "CatchState"; }

private:
	AIComponent* aiComponent = nullptr;
	TransformComponent* transformComponent = nullptr;
	VelocityComponent* velocityComponent = nullptr;
	MeshComponent* meshComponent = nullptr;

	Entity* playerEntity = nullptr;
	Entity* aiEntity = nullptr;
};

class StateMachine
{
public:
	StateMachine();
	~StateMachine();

	void AddTransition(BehaviourType from, BehaviourType to);
	void Update(float dt, Entity* playerEntity);
	State* GetCurrentState();
	void SetState(State* newState, Entity* entity);
	void SetCatchTimer(int timer);

private:
	std::map<BehaviourType, std::vector<BehaviourType>> m_ValidTransitions;

	Entity* entity;
	AIComponent* aiComponent;
	State* m_CurrentState;

	int catchTimer;
};

class AISystem : public System
{
public:
	// Constructor
	AISystem();

	// Destructor
	~AISystem();

	// The update method called every tick
	void Process(const std::vector<Entity*>& entities, float dt);

	// Pursue Steering behaviour
	void Pursue(float dt, float distance, glm::vec3 playerPosition, AIComponent* aiComponent, TransformComponent* transformComponent);

	// Gracefully closes everything down
	void Shutdown();

private:
	// Enemy face the direction of player
	void Face(glm::vec3 targetPosition, TransformComponent* transformComponent);

	// Get the distance between player and enemy
	float GetDistance(glm::vec3 aiPosition, glm::vec3 playerPosition);

	// Get the player components
	TransformComponent* GetPlayerTransform(const std::vector<Entity*>& entities);
	VelocityComponent* GetPlayerVelocity(const std::vector<Entity*>& entities);
	MeshComponent* GetPlayerMesh(const std::vector<Entity*>& entities);

	Entity* GetPlayerEntity(const std::vector<Entity*>& entities);
	PlayerComponent* GetPlayerComponent(const std::vector<Entity*>& entities);

	StateMachine* stateMachine;

	IdleState* idleState;
	PursueState* pursueState;
	CatchState* catchState;
};

class StatePool {
public:
	StatePool(int size) {
		for (int i = 0; i < size; i++) {
			pool.push_back(new IdleState());
		}
	}

	State* GetState() {
		if (pool.empty()) {
			pool.push_back(new IdleState());
		}
		State* state = pool.back();
		pool.pop_back();
		return state;
	}

	void ReturnState(State* state) {
		pool.push_back(state);
	}

	~StatePool() {
		for (auto state : pool) {
			delete state;
		}
	}

private:
	std::vector<State*> pool;
};