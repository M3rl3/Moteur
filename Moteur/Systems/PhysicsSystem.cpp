#include "PhysicsSystem.h"

#include <../PhysicsEngine/include/PhysicsFactory.h>
#include <../PhysicsEngine/interfaces/iRigidBody.h>

#include "../Components/RigidBodyComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"

// Constructor
PhysicsSystem::PhysicsSystem()
{
    // Class type used here
    // Init physics factory
    physicsFactory = new physics::PhysicsFactory();

    // Init physics world
    physicsWorld = physicsFactory->CreateWorld();

    // Set Gravity
    physicsWorld->SetGravity(glm::vec3(0.f, -9.8f, 0.f));
    // physicsWorld->SetGravity(glm::vec3(0.f, 0.f, 0.f));
}

// Destructor
PhysicsSystem::~PhysicsSystem() {
    
}

// Create a rigid body
void PhysicsSystem::CreateRigidBody(const physics::RigidBodyDesc& desc, physics::iShape* shape)
{
    physicsFactory->CreateRigidBody(desc, shape);
}

// Add a body to the physics system
void PhysicsSystem::AddBody(physics::iCollisionBody* body)
{
    physicsWorld->AddBody(body);
}

// Set the gravitational constant
void PhysicsSystem::SetGravity(const glm::vec3 gravity)
{
    physicsWorld->SetGravity(gravity);
}

// Process all entities that have physics applied
void PhysicsSystem::Process(const std::vector<Entity*>& entities, float dt)
{
    // Make a copy of all the entity components
    RigidBodyComponent* rigidBodyComponent = nullptr;
    TransformComponent* transformComponent = nullptr;
    VelocityComponent* velocityComponent = nullptr;

    // Iterate through all entities
    for (int i = 0; i < entities.size(); i++) {

        Entity* currentEntity = entities[i];

        // get the specific instances for all components
        rigidBodyComponent = currentEntity->GetComponentByType<RigidBodyComponent>();
        transformComponent = currentEntity->GetComponentByType<TransformComponent>();
        velocityComponent = currentEntity->GetComponentByType<VelocityComponent>();

        // Check if the required components exist
        if (rigidBodyComponent != nullptr && transformComponent != nullptr) {

            // Create and add the entity rigid bodies
            if (rigidBodyComponent->isInfluenced && rigidBodyComponent->doOnce) {

                // Set initital position
                rigidBodyComponent->rigidBodyDesc.position = transformComponent->position;
                
                // Initialize the rigid body
                rigidBodyComponent->rigidBody =
                    physicsFactory->CreateRigidBody(rigidBodyComponent->rigidBodyDesc, 
                                                    rigidBodyComponent->bodyShape);

                // Add the rigid body to the physics world
                physicsWorld->AddBody(rigidBodyComponent->rigidBody);

                // Only add all bodies once
                rigidBodyComponent->doOnce = false;
            }

            // Check if the object should be under the influence of physics
            if (rigidBodyComponent->isInfluenced) {
                // convert all collision bodies to rigid bodies
                physics::iRigidBody* rigidBody =
                    dynamic_cast<physics::iRigidBody*>(rigidBodyComponent->rigidBody);

                glm::vec3 position;

                rigidBody->GetPosition(position);
                rigidBody->GetRotation(transformComponent->rotation);

                transformComponent->position = position;

                if (velocityComponent != nullptr && !velocityComponent->useVelocity) {
                    rigidBody->ApplyForce(velocityComponent->velocity);
                }
            }
            else {
                // Move on
                continue;
            }
        }
    }

    // Update all physics objects
    physicsWorld->TimeStep(dt);
}

// Shutdown method
void PhysicsSystem::Shutdown()
{
    physicsWorld = nullptr;
    delete physicsWorld;

    physicsFactory = nullptr;
    delete physicsFactory;
}

// Getter for the physics world
physics::iPhysicsWorld* PhysicsSystem::GetPhysicsWorld()
{
    return physicsWorld;
}

// Getter for the physics factory
physics::iPhysicsFactory* PhysicsSystem::GetPhysicsFactory()
{
    return physicsFactory;
}