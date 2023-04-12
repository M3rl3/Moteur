#pragma once

#include "../interfaces/iPhysicsFactory.h"

#include <bullet/BulletDynamics/Dynamics/btDynamicsWorld.h>

namespace physics {
	class PhysicsFactory : public iPhysicsFactory
	{
	public:
		PhysicsFactory();
		virtual ~PhysicsFactory();

		virtual iPhysicsWorld* CreateWorld() override;
		virtual iRigidBody* CreateRigidBody(const RigidBodyDesc& desc, iShape* shape) override;
		virtual iSoftBody* CreateSoftBody(const SoftBodyDesc& desc) override;
		virtual iCharacterController* CreateCharacterController(iConvexShape* shape, float stepHeight, const Vector3& up) override;

	private:
		btDynamicsWorld* dynamicsWorld;
	};
}