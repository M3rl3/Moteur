#pragma once

#include <glm/glm.hpp>
#include <ECS/System.h>

#include "../Components/AIComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/MeshComponent.h"

class State
{
public:
	State() { }
	State(BehaviourType type) : m_StateType(type) { }
	virtual ~State() { }
	virtual BehaviourType GetType() const { return m_StateType; }
	virtual void Enter(Entity* entity) = 0;
	virtual void Update(Entity* entity) = 0;
	virtual void Exit(Entity* entity) = 0;
	virtual const char* C_Str() = 0;

private:
	BehaviourType m_StateType;
};

class IdleState : public State
{
public:
	IdleState();
	virtual ~IdleState();

	virtual void Enter(Entity* entity) override;
	virtual void Update(Entity* entity) override;
	virtual void Exit(Entity* entity) override;
	virtual const char* C_Str() override { return "IdleState"; }

private:
	AIComponent* aiComponent = nullptr;
	TransformComponent* transformComponent = nullptr;
	VelocityComponent* velocityComponent = nullptr;
	MeshComponent* meshComponent = nullptr;
};

class PursueState : public State
{
public:
	PursueState();
	virtual ~PursueState();

	virtual void Enter(Entity* entity) override;
	virtual void Update(Entity* entity) override;
	virtual void Exit(Entity* entity) override;
	virtual const char* C_Str() override { return "PursueState"; }

private:
	AIComponent* aiComponent = nullptr;
	TransformComponent* transformComponent = nullptr;
	VelocityComponent* velocityComponent = nullptr;
	MeshComponent* meshComponent = nullptr;
};

class CatchState : public State
{
public:
	CatchState();
	virtual ~CatchState();

	virtual void Enter(Entity* entity) override;
	virtual void Update(Entity* entity) override;
	virtual void Exit(Entity* entity) override;
	virtual const char* C_Str() override { return "CatchState"; }

private:
	AIComponent* aiComponent = nullptr;
	TransformComponent* transformComponent = nullptr;
	VelocityComponent* velocityComponent = nullptr;
	MeshComponent* meshComponent = nullptr;
};

class StateMachine
{
public:
	StateMachine();
	~StateMachine();

	void AddTransition(BehaviourType from, BehaviourType to);
	State* GetCurrentState();
	void SetState(State* state);

private:
	std::map<BehaviourType, std::vector<BehaviourType>> m_ValidTransitions;

	Entity* entity;
	State* m_CurrentState;
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

	StateMachine* stateMachine;
};
