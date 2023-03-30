#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

// Simple struct holding camera position and lookAt
class Camera {
public:

	// Constructor
	Camera();

	// Constructor
	Camera(float left, float right, float bottom, float top);

	// Destructor
	~Camera();

	// Camera position
	glm::vec3 position;

	// Camera lookAt
	glm::vec3 target;

	const glm::vec3 upVector;

	// Camera rotation
	glm::quat rotation;

	// view matrix
	glm::mat4 view;

	// projection matrix
	glm::mat4 projection;

	// view projection matrix
	glm::mat4 viewProjection;

	const float movementSpeed;
public:
	void MoveUp();
	void MoveDown();
	void MoveForward();
	void MoveBackward();
	void StrafeLeft();
	void StrafeRight();

private:
	void RecalculateViewMatrix();
	
};