#pragma once

#include <ECS/System.h>

#include "TransformComponent.h"

#include "AnimationManager.h"
#include "cVAOManager/cVAOManager.h"

#include "Window.h"
#include "Camera.h"

// The system that handles rendering the scene
class RenderSystem : public System {
public:

	// Constructor
	RenderSystem();
	// Destructor
	~RenderSystem();

	// Creates a window based on the size and title provided
	void CreateWindow(const char* title, const int width, const int height, bool fullScreen);

	// Initializes GLFW and the window
	void Initialize(const char* title, const int width, const int height, bool fullScreen);

	// Loads model from ply file and gets it into the VAO
	bool LoadMesh(std::string fileName, std::string modelName, sModelDrawInfo& plyModel, unsigned int shaderID);

	// Update method called every tick
	void Process(const std::vector<Entity*>& entities, float dt);

	// Gracefully closes everything down
	void Shutdown();

	// Returns the instance of the window
	Window* GetWindow();

	// Returns the instance of the camera
	Camera* GetCamera();

	// Sets the initial camera position
	void SetCameraPosition(glm::vec3 pos);

	// Sets the initial camera lookAt
	void SetCameraTarget(glm::vec3 tar);
	
private:

	// name of the system
	std::string systemName;

	// Pointer to window
	Window* window;

	// Pointer to camera
	Camera* camera;

	// Handler for all stuff VAO related
	cVAOManager* vaoManager;

	// Handler for all stuff animation related
	AnimationManager* animationManager;
};