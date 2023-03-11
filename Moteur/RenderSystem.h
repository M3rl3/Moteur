#pragma once

#include <ECS/System.h>

#include "TransformComponent.h"

#include "AnimationManager.h"
#include "cPlyFileLoader.h"

#include "cVAOManager/cVAOManager.h"
#include "cBasicTextureManager/cBasicTextureManager.h"

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

	// Set the path where the textures are located
	void SetTexturePath(const char* filePath);

	// Load a 2D texture
	bool Load2DTexture(unsigned int& textureID, const char* filePath);

	// Load a skybox texture
	bool LoadCubeMapTexture(
		unsigned int& textureID,
		std::string cubeMapName,
		std::string posX_fileName, std::string negX_fileName,
		std::string posY_fileName, std::string negY_fileName,
		std::string posZ_fileName, std::string negZ_fileName,
		bool bIsSeamless, std::string& errorString);

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

	bool* GetKeyPressedArray();

	// User Input
	bool IsKeyHeldDown(unsigned char key);
	bool IsKeyPressed(unsigned char key);
	bool IsKeyReleased(unsigned char key);

	// Keyboard Callback
	static void ProcessInput(GLFWwindow* window, int key, int scancode, int action, int mods);
	
private:

	// name of the system
	std::string systemName;

	// Pointer to window
	Window* window;

	// Pointer to camera
	Camera* camera;

	// Ply file loader
	cPlyFileLoader* plyFileLoader;

	// Handler for all VAO related stuff
	cVAOManager* vaoManager;

	// Handler for all Texture related stuff
	cBasicTextureManager* textureManager;

	// Handler for all stuff animation related
	AnimationManager* animationManager;
};