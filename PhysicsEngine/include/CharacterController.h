#pragma once

#include "iCharacterController.h"
#include "GLMath.h"
#include "iShape.h"
#include "iPhysicsWorld.h"

#include <bullet/BulletDynamics/Character/btKinematicCharacterController.h>

namespace physics
{
	class CharacterController : public iCharacterController
	{
	public:
		CharacterController(btPairCachingGhostObject* btGhostObject, 
				btConvexShape* shape, btScalar stepHeight, const btVector3& up);
		virtual ~CharacterController() override;

		virtual void SetWalkDirection(const Vector3& walkDirection) override;
		virtual void SetVelocityForTimeInterval(const Vector3& velocity, float timeInterval) override;
		virtual void Warp(const Vector3& origin) override;
		virtual bool CanJump() const override;
		virtual void Jump(const Vector3& dir = Vector3(0, 0, 0)) override;
		virtual bool OnGround() const override;
		virtual void UpdateAction(iPhysicsWorld* collisionWorld, float dt) override;
		virtual void GetTransform(glm::mat4& transform) override;
		virtual void SetTransform(const Matrix4x4& transform) override;

		btPairCachingGhostObject* GetGhostObject();
		btCharacterControllerInterface* GetBulletCharacterController();

	private:
		CharacterController();

		btPairCachingGhostObject* ghostObject;
		btKinematicCharacterController* characterController;

		CharacterController(const CharacterController& other) { }
		CharacterController& operator=(const CharacterController& other0) {
			return *this;
		}
	};
}