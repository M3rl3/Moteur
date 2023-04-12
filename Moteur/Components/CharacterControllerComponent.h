#pragma once

#include <ECS/Component.h>
#include <glm/vec3.hpp>

#include "../PhysicsEngine/interfaces/iCharacterController.h"
#include "../PhysicsEngine/interfaces/iShape.h"

// struct holding character controller info
struct CharacterControllerComponent : Component {
public:

	// Constructor
	CharacterControllerComponent() :
		componentType("CharacterControllerComponent"),
		convexShape(nullptr),
		characterController(nullptr),
		stepHeight(1.f),
		up(glm::vec3(0.f, 1.f, 0.f)),
		isControllable(true),
		canJump(true),
		doOnce(true)
	{};

	// Destructor
	~CharacterControllerComponent() {};

	// Shape of the colliding body
	physics::iConvexShape* convexShape;

	// Instance of the character controller interface
	physics::iCharacterController* characterController;

	// How high the character can step up
	float stepHeight;

	// Up direction
	const glm::vec3 up;

	// If the controller is in use
	bool isControllable;

	// If the character can jump
	bool canJump;

	// throwaway variable
	bool doOnce;

private:
	// name of the component
	std::string componentType;
};