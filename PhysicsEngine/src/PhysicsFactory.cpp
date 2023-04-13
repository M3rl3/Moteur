#include "../include/PhysicsFactory.h"
#include "../include/PhysicsWorld.h"
#include "../include/RigidBody.h"
#include "../include/Conversion.h"

#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

namespace physics {
	PhysicsFactory::PhysicsFactory() : iPhysicsFactory() {

	}

	PhysicsFactory::~PhysicsFactory() {

	}

	iPhysicsWorld* PhysicsFactory::CreateWorld()
	{
		PhysicsWorld* world = new PhysicsWorld();

		dynamicsWorld = world->GetDynamicsWorld();
		return world;
	}

	iRigidBody* PhysicsFactory::CreateRigidBody(const RigidBodyDesc& desc, iShape* shape)
	{
		return new RigidBody(desc, shape);
	}

	iSoftBody* PhysicsFactory::CreateSoftBody(const SoftBodyDesc& desc)
	{
		return nullptr;
	}

	iCharacterController* PhysicsFactory::CreateCharacterController(
		iConvexShape* shape, float stepHeight, const Vector3& up)
	{
		btPairCachingGhostObject* btGhostObject = new btPairCachingGhostObject();
		btConvexShape* btShape;
		btScalar btStepHeight;
		btVector3 btUp;

		btShape = CastBulletConvexShape(shape);
		CastBulletScalar(stepHeight, &btStepHeight);
		CastBulletVector3(up, &btUp);

		iCharacterController* cc = new CharacterController(btGhostObject, btShape, btStepHeight, btUp);

		// Do this after Creating the Character Controller
		btGhostObject->setCollisionShape(btShape);
		btGhostObject->setWorldTransform(btTransform(btQuaternion(0, 0, 0, 1), btVector3(-5, 5, -5)));

		return cc;
	}

	iCharacterController* PhysicsFactory::CreateCharacterController(iConvexShape* shape, float stepHeight, const Vector3& up, const Matrix4x4& model)
	{
		btPairCachingGhostObject* btGhostObject = new btPairCachingGhostObject();
		btConvexShape* btShape;
		btScalar btStepHeight;
		btVector3 btUp;
		btTransform btModel;

		btShape = CastBulletConvexShape(shape);
		CastBulletScalar(stepHeight, &btStepHeight);
		CastBulletVector3(up, &btUp);
		CastBulletMatrix4x4(model, &btModel);

		iCharacterController* cc = new CharacterController(btGhostObject, btShape, btStepHeight, btUp);

		// Do this after Creating the Character Controller
		btGhostObject->setCollisionShape(btShape);
		btGhostObject->setWorldTransform(btModel);

		return cc;
	}
}