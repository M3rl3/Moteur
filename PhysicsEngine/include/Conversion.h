#pragma once

// Bullet Source
#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletDynamics/Character/btCharacterControllerInterface.h>

// Physics Interfaces
#include <iShape.h>
#include <iCollisionBody.h>
#include <iCharacterController.h>

// Math Library
#include <glm/gtx/quaternion.hpp>

namespace physics
{
	// GLM -> Bullet
	void CastBulletQuaternion(const glm::quat& in, btQuaternion* out);
	void CastBulletVector3(const glm::vec3& in, btVector3* out);
	void CastBulletMatrix4x4(const glm::mat4x4& in, btTransform* out);
	void CastBulletScalar(const float in, btScalar* out);

	// Bullet -> GLM
	void CastGLMQuat(const btQuaternion& in, glm::quat* out);
	void CastGLMVec3(const btVector3& in, glm::vec3* out);
	void CastGLMMat4(const btTransform& in, glm::mat4* out);
	void CastFloat(const btScalar& in, float* out);

	// Interface -> Bullet
	btCollisionShape* CastBulletShape(iShape* shape);
	btDiscreteDynamicsWorld* CastBulletWorld(iPhysicsWorld* world);
	btRigidBody* CastBulletRigidBody(iCollisionBody* body);
	btConvexShape* CastBulletConvexShape(iConvexShape* shape);
	btCharacterControllerInterface* CastBulletCharacterController(
		iCharacterController* characterController);
};