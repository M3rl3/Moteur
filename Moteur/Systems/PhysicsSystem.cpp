#include "PhysicsSystem.h"

#include <../PhysicsEngine/include/PhysicsFactory.h>
#include <../PhysicsEngine/interfaces/iRigidBody.h>

#include "../Components/RigidBodyComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/AIComponent.h"
#include "../Components/PlayerComponent.h"
#include "../Components/CharacterControllerComponent.h"

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

// Create a character controller
void PhysicsSystem::CreateCharacterController(physics::iConvexShape* shape, float stepHeight, const glm::vec3& up)
{
    physicsFactory->CreateCharacterController(shape, stepHeight, up);
}

// Add a body to the physics world
void PhysicsSystem::AddBody(physics::iCollisionBody* body)
{
    physicsWorld->AddBody(body);
}

// Add a character controller to the physics world
void PhysicsSystem::AddCharacterController(physics::iCharacterController* characterController)
{
    physicsWorld->AddCharacterController(characterController);
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
    CharacterControllerComponent* characterControllerComponent = nullptr;
    AIComponent* aiComponent = nullptr;
    PlayerComponent* playerComponent = nullptr;

    // Iterate through all entities
    for (int i = 0; i < entities.size(); i++) {

        Entity* currentEntity = entities[i];

        // get the specific instances for all components
        rigidBodyComponent = currentEntity->GetComponentByType<RigidBodyComponent>();
        transformComponent = currentEntity->GetComponentByType<TransformComponent>();
        velocityComponent = currentEntity->GetComponentByType<VelocityComponent>();
        aiComponent = currentEntity->GetComponentByType<AIComponent>();
        playerComponent = currentEntity->GetComponentByType<PlayerComponent>();
        characterControllerComponent = currentEntity->GetComponentByType<CharacterControllerComponent>();

        // Check if a rigid body component exists
        if (rigidBodyComponent != nullptr && transformComponent != nullptr) {

            // Create and add the entity rigid bodies
            if (rigidBodyComponent->usePhysics && rigidBodyComponent->doOnce) {

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
            if (rigidBodyComponent->usePhysics) {

                // convert all collision bodies to rigid bodies
                physics::iRigidBody* rigidBody =
                    dynamic_cast<physics::iRigidBody*>(rigidBodyComponent->rigidBody);

                glm::vec3 position;
                glm::quat rotation;

                // Update the transformation on the visual side
                rigidBody->GetPosition(position);
                rigidBody->GetRotation(rotation);

                // transformComponent->rotation = rotation;
                transformComponent->position = position;

                if (velocityComponent != nullptr && !velocityComponent->useVelocity) {
                    rigidBody->ApplyForce(velocityComponent->velocity * 5.f);
                }
            }
        }

        // Check if a character controller component exists
        if (characterControllerComponent != nullptr && transformComponent != nullptr) {
            
            // Create and add the character controller
            if (characterControllerComponent->isControllable && characterControllerComponent->doOnce) {

                // Initialize the character controller
                characterControllerComponent->characterController =
                    physicsFactory->CreateCharacterController(characterControllerComponent->convexShape,
                        characterControllerComponent->stepHeight, characterControllerComponent->up,
                            transformComponent->initMatModel);  
                
                // Add the character controller to the physics world
                physicsWorld->AddCharacterController(characterControllerComponent->characterController);

                // pass in the initial model matrix from transform values
                // characterControllerComponent->characterController->SetTransform(transformComponent->initMatModel);

                // Do this only once per character controller
                characterControllerComponent->doOnce = false;
            }

            // If a character is controllable
            if (characterControllerComponent->isControllable) {

                // Get the velocity of that guy
                if (velocityComponent != nullptr && !velocityComponent->useVelocity) {

                    if (playerComponent->isControllable) {
                        // Set the direction to whatever velocity value that is coming in
                        characterControllerComponent->characterController->SetWalkDirection(velocityComponent->velocity * dt);

                        // Check if the character can jump
                        if (characterControllerComponent->canJump && velocityComponent->velocity.y != 0.f) {
                            characterControllerComponent->characterController->Jump(velocityComponent->velocity);
                        }
                    }

                    // Update the transformation on the visual side
                    if (transformComponent->useModel) {
                        // If the model matrix is in use
                        characterControllerComponent->characterController->GetTransform(transformComponent->matModel);
                    }
                }

                // Call update action on the character controller
                // characterControllerComponent->characterController->UpdateAction(physicsWorld, dt);
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