#pragma once

#include <ECS/Component.h>

#include <../PhysicsEngine/interfaces/RigidBodyDesc.h>
#include <../PhysicsEngine/interfaces/iCollisionBody.h>
#include <../PhysicsEngine/interfaces/iPhysicsWorld.h>
#include <../PhysicsEngine/interfaces/iShape.h>

// Struct holding rigid body and shape info
struct RigidBodyComponent : Component {
public:

	// Constructor
	RigidBodyComponent() :
		componentType("RigidBodyComponent"),
		bodyShape(nullptr),
		rigidBody(nullptr),
		usePhysics(true),
		doOnce(true)
	{};

	// Destructor
	~RigidBodyComponent() {};

	// Shape of the colliding body
	physics::iShape* bodyShape;

	// Struct holding rigid body metadata
	physics::RigidBodyDesc rigidBodyDesc;

	// The actual colliding body
	physics::iCollisionBody* rigidBody;

	// If the object is influenced by physics
	bool usePhysics;

	// Add the physics body only once
	bool doOnce;

private:
	// name of the component
	std::string componentType;
};