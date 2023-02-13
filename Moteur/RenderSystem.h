#pragma once

#include <ECS/System.h>

#include "TransformComponent.h"

#include "AnimationManager.h"
#include "cVAOManager/cVAOManager.h"

#include "Window.h"
#include "Camera.h"

class RenderSystem : public System {
public:

	RenderSystem();
	~RenderSystem();

	void CreateWindow(const char* title, const int width, const int height, bool fullScreen);
	void Initialize(const char* title, const int width, const int height, bool fullScreen);

	bool LoadMesh(std::string fileName, std::string modelName, sModelDrawInfo& plyModel, unsigned int shaderID);
	void Process(const std::vector<Entity*>& entities, float dt);
	void Shutdown();

	Window* GetWindow();
	Camera* GetCamera();

	void SetCameraPosition(glm::vec3 pos);
	void SetCameraTarget(glm::vec3 tar);
	
private:

	std::string systemName;

	Window* window;
	Camera* camera;

	cVAOManager* vaoManager;
	AnimationManager* animationManager;
};