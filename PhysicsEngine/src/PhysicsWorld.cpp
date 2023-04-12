#include "PhysicsWorld.h"
#include "Conversion.h"

#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

#include <iostream>

namespace physics {

	PhysicsWorld::PhysicsWorld() : 
		iPhysicsWorld()
	{
		collisionConfiguration = new btDefaultCollisionConfiguration();
		dispatcher = new btCollisionDispatcher(collisionConfiguration);
		broadphase = new btDbvtBroadphase();
		solver = new btSequentialImpulseConstraintSolver;
		dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,
			broadphase, solver, collisionConfiguration);
	}

	PhysicsWorld::PhysicsWorld(const PhysicsWorld&) {

	}
	
	PhysicsWorld::~PhysicsWorld() 
	{
		delete dynamicsWorld;
		delete collisionConfiguration;
		delete dispatcher;
		delete broadphase;
		delete solver;	
	}

	void PhysicsWorld::SetGravity(const glm::vec3& gravity)
	{
		btVector3 btGravity;
		CastBulletVector3(gravity, &btGravity);
		dynamicsWorld->setGravity(btGravity);
	}

	void PhysicsWorld::AddBody(iCollisionBody* body)
	{
		btRigidBody* bulletBody = CastBulletRigidBody(body);
		dynamicsWorld->addRigidBody(bulletBody);
	}

	void PhysicsWorld::RemoveBody(iCollisionBody* body)
	{
		btRigidBody* bulletBody = CastBulletRigidBody(body);
		dynamicsWorld->removeRigidBody(bulletBody);
	}

	void PhysicsWorld::AddCharacterController(iCharacterController* characterController)
	{
		btCharacterControllerInterface* btCharacterController =
			CastBulletCharacterController(characterController);
		CharacterController* cc = dynamic_cast<CharacterController*>(characterController);

		btPairCachingGhostObject* btGhostObject = cc->GetGhostObject();
		dynamicsWorld->addCollisionObject(btGhostObject,
			btBroadphaseProxy::CharacterFilter,
			btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);

		dynamicsWorld->addAction(btCharacterController);
	}

	void PhysicsWorld::RemoveCharacterController(iCharacterController* characterController)
	{
		CharacterController* cc = dynamic_cast<CharacterController*>(characterController);
		btPairCachingGhostObject* btGhostObject = cc->GetGhostObject();
		btCharacterControllerInterface* btCharacterController =
			CastBulletCharacterController(characterController);
		dynamicsWorld->removeAction(btCharacterController);
		dynamicsWorld->removeCollisionObject(btGhostObject);
	}

	void PhysicsWorld::TimeStep(float dt)
	{
		dynamicsWorld->stepSimulation(dt);
	}

	btDiscreteDynamicsWorld* PhysicsWorld::GetDynamicsWorld(void) 
	{
		return dynamicsWorld;
	}

	void PhysicsWorld::DebugDraw()
	{
		return;
	}

	void PhysicsWorld::RegisterCollisionListener(iCollisionListener* listener) {
		return;
	}
}

