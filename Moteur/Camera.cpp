#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

// Constructor
Camera::Camera() :
	position(glm::vec3(0.f)),
	rotation(glm::quat(glm::vec3(0.f))),
	target(glm::vec3(0.f)),
	view(glm::mat4(1.f)),
	projection(glm::mat4(1.f)),
	viewProjection(glm::mat4(1.f)),
	upVector(glm::vec3(0.f, 1.f, 0.f)),
	movementSpeed(1.f)
{}

// Constructor for orthographic camera
Camera::Camera(float left, float right, float bottom, float top) :
	position(glm::vec3(0.f)),
	rotation(glm::quat(glm::vec3(0.f))),	
	target(glm::vec3(0.f)),
	view(glm::mat4(1.f)),
	projection(glm::ortho(left, right, bottom, top, -1.f, 1.f)),
	viewProjection(glm::mat4(1.f)),
	upVector(glm::vec3(0.f, 1.f, 0.f)),
	movementSpeed(1.f)
{}

// Destructor
Camera::~Camera() 
{}

void Camera::RecalculateViewMatrix()
{
	glm::mat4 transform = glm::translate(glm::mat4(1.f), position) * glm::mat4(rotation);

	view = glm::inverse(transform);
	viewProjection = projection * view;				  
}

void Camera::MoveUp() {
	position += movementSpeed * upVector;
}

void Camera::MoveDown() {
	position += -movementSpeed * upVector;
}

void Camera::MoveForward() {
	position += movementSpeed * target;
}

void Camera::MoveBackward() {
	position += -movementSpeed * target;
}

void Camera::StrafeLeft() {
	glm::vec3 strafeDirection = glm::cross(target, upVector);
	position += -movementSpeed * strafeDirection;
}

void Camera::StrafeRight() {
	glm::vec3 strafeDirection = glm::cross(target, upVector);
	position += movementSpeed * strafeDirection;
}