#pragma once

#include <glm/glm.hpp>
#include <ECS/System.h>

#include "../Components/AIComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/MeshComponent.h"
#include "../Components/CharacterControllerComponent.h"

class StatePool;

class IdleState : public State
{
public:
	IdleState();
	virtual ~IdleState();

	virtual const char* GetType() const override;
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

	std::string typeName;
};

class PursueState : public State
{
public:
	PursueState();
	virtual ~PursueState();

	virtual const char* GetType() const override;
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

	std::string typeName;
};

class CatchState : public State
{
public:
	CatchState();
	virtual ~CatchState();

	virtual const char* GetType() const override;
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

	std::string typeName;
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
	void SetState(State* newState, StatePool* statePool, Entity* aiEntity, Entity* playerEntity);
	void SetCatchTimer(int timer);

private:
	std::map<BehaviourType, std::vector<BehaviourType>> m_ValidTransitions;

	AIComponent* aiComponent;

	TransformComponent* aiTransform;
	TransformComponent* playerTransform;

	VelocityComponent* aiVelocity;

	PlayerComponent* playerComponent;
	CharacterControllerComponent* characterController;

	State* m_CurrentState;
	CatchState* catchState;

	
	int catchTimer, catchDuration, catchStartTime;
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
	CharacterControllerComponent* GetCharacterController(const std::vector<Entity*>& entities);

	StateMachine* stateMachine;
	StatePool* statePool;
};

class StatePool {
public:
	// Preferably a number divisible by 2
	StatePool(int size);
	~StatePool();

	State* GetState();
	
	template<class T>
	T* GetState();

	void ReturnState(State* state);

	std::vector<State*>& GetStatePool();

private:
	std::vector<State*> pool;
};