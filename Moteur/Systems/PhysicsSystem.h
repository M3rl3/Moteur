#pragma once

#include <ECS/System.h>

#include <../PhysicsEngine/interfaces/iPhysicsFactory.h>
#include <../PhysicsEngine/interfaces/iPhysicsWorld.h>
#include <../PhysicsEngine/interfaces/iCollisionBody.h>
#include <../PhysicsEngine/interfaces/RigidBodyDesc.h>
#include <../PhysicsEngine/interfaces/iShape.h>

#include <glm/vec3.hpp>

// The system handling all physics objects
class PhysicsSystem : public System {
public:

	// Constructor
	PhysicsSystem();

	// Destructor
	~PhysicsSystem();

	// Create a rigid body
	void CreateRigidBody(const physics::RigidBodyDesc& desc, physics::iShape* shape);

	// Create a character controller
	void CreateCharacterController(physics::iConvexShape* shape, float stepHeight, const glm::vec3& up);

	// Add a body to the physics world
	void AddBody(physics::iCollisionBody* body);

	// Add a character controller to the physics world
	void AddCharacterController(physics::iCharacterController* characterController);

	// Set the gravitational constant
	void SetGravity(const glm::vec3 gravity);

	// The update method called every tick
	void Process(const std::vector<Entity*>& entities, float dt);

	// Shutdown method
	void Shutdown();

	// Getter for the physics world
	physics::iPhysicsWorld* GetPhysicsWorld();
	
	// Getter for the physics factory
	physics::iPhysicsFactory* GetPhysicsFactory();

private:
	// Physics world instance
	physics::iPhysicsWorld* physicsWorld;

	// Physics factory instance
	physics::iPhysicsFactory* physicsFactory;
};