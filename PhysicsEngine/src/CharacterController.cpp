#include "CharacterController.h"
#include "Conversion.h"

#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <bullet/BulletCollision/btBulletCollisionCommon.h>

namespace physics
{
	CharacterController::CharacterController() 
	{
	}

	CharacterController::CharacterController(
		btPairCachingGhostObject* btGhostObject,
		btConvexShape* shape,
		btScalar stepHeight,
		const btVector3& up)
		: ghostObject(btGhostObject)
	{
		characterController = new btKinematicCharacterController(
			ghostObject, shape, stepHeight, up);
		characterController->setUseGhostSweepTest(false);

		characterController->setMaxPenetrationDepth(0);
	}

	CharacterController::~CharacterController()
	{
	}

	void CharacterController::SetWalkDirection(const Vector3& walkDirection)
	{
		btVector3 btWalkDirection;
		CastBulletVector3(walkDirection, &btWalkDirection);
		characterController->setWalkDirection(btWalkDirection);
	}

	void CharacterController::SetVelocityForTimeInterval(const Vector3& velocity, float timeInterval)
	{
		btVector3 btVelocity;
		CastBulletVector3(velocity, &btVelocity);
		characterController->setVelocityForTimeInterval(btVelocity, btScalar(timeInterval));
	}

	void CharacterController::Warp(const Vector3& origin)
	{
		btVector3 btOrigin;
		CastBulletVector3(origin, &btOrigin);
		characterController->warp(btOrigin);
	}

	bool CharacterController::CanJump() const
	{
		return characterController->canJump();
	}

	void CharacterController::Jump(const Vector3& dir/*=Vector3(0, 0, 0)*/)
	{
		btVector3 btDir;
		CastBulletVector3(dir, &btDir);
		characterController->jump(btDir);
	}

	bool CharacterController::OnGround() const
	{
		return characterController->onGround();
	}

	void CharacterController::UpdateAction(iPhysicsWorld* collisionWorld, float dt)
	{
		btDiscreteDynamicsWorld* btCollisionWorld = CastBulletWorld(collisionWorld);
		characterController->updateAction(btCollisionWorld, dt);
	}

	btPairCachingGhostObject* CharacterController::GetGhostObject()
	{
		return ghostObject;
	}

	btCharacterControllerInterface* CharacterController::GetBulletCharacterController()
	{
		return characterController;
	}
}