#pragma once

#include "ECS/Component.h"

#include <glm/vec3.hpp>

enum BehaviourType
{
	IDLE,
	SEEK,
	FLEE,
	PURSUE,
	EVADE,
	APPROACH,
	CATCH
};

// Interface
class State
{
public:
	State() : m_StateType(IDLE) { }
	State(BehaviourType type) : m_StateType(type) { }
	virtual ~State() { }
	virtual const char* GetType() const = 0;
	virtual std::string GetTypeName() const { return m_TypeName; }
	virtual void Enter(Entity* entity) = 0;
	virtual void Update(float dt, Entity* playerEntity) = 0;
	virtual void Exit() = 0;
	virtual const char* C_Str() = 0;

private:
	BehaviourType m_StateType;
	std::string m_TypeName;
};


struct AIComponent : Component
{
public:
	AIComponent() :
		ComponentType("AIComponent"),
		aiBehaviour(BehaviourType::SEEK),
		speed(0.f),
		catchTimer(0.f),
		radius(0.f),
		aiVelocity(glm::vec3(0)),
		currentState(nullptr),
		doOnce(true),
		aiActive(true)
	{};

	// The destructor
	~AIComponent() {};

	// Trigger radius between player and AI agent
	float radius;

	float catchTimer;

	// Movement speed
	float speed;

	// a temp velocity
	glm::vec3 aiVelocity;

	// The behavior type
	BehaviourType aiBehaviour;

	// The ai state
	State* currentState;

	bool doOnce;

	bool aiActive;

private:
	// The component type
	std::string ComponentType;
};
