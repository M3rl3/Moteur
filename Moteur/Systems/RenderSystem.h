#pragma once

#include <ECS/System.h>

#include "../Components/TransformComponent.h"
#include "../Components/MeshComponent.h"

#include "../cModelFileLoader/cModelFileLoader.h"

#include "../cVAOManager/cVAOManager.h"
#include "../cTextureManager/cTextureManager.h"

#include "../Window/Window.h"
#include "../Camera/Camera.h"

#include "GUISystem.h"

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
	
	// Set the path where the meshes are located
	void SetMeshPath(std::string filePath);

	// Loads model from ply file and gets it into the VAO
	bool LoadMesh(std::string fileName, std::string modelName, sModelDrawInfo& plyModel, unsigned int shaderID);

	// Loads model from ply file and gets it into the VAO
	bool LoadMesh(std::string fileName, std::string modelName, ModelType modelType, unsigned int shaderID);

	// Set the path where the textures are located
	void SetTexturePath(std::string filePath);

	// Load a BMP 2D texture
	bool Load2DTextureBMP(unsigned int& textureID, const char* filePath);

	// Load a BMP 2D texture
	bool Load2DTextureBMP(const char* filePath);

	// Load a PNG 2D texture
	bool Load2DTexturePNG(unsigned int& textureID, const char* filePath);

	// Load a PNG 2D texture
	bool Load2DTexturePNG(const char* filePath);

	// Load a BMP skybox texture
	bool LoadCubeMapTextureBMP(
		unsigned int& textureID,
		std::string cubeMapName,
		std::string posX_fileName, std::string negX_fileName,
		std::string posY_fileName, std::string negY_fileName,
		std::string posZ_fileName, std::string negZ_fileName,
		bool bIsSeamless, std::string& errorString);
	
	// Load a PNG skybox texture
	bool LoadCubeMapTexturePNG(
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
	static void ProcessMouseMovement(GLFWwindow* window, double xposition, double yposition);

	// Get mouse enbled status
	bool GetMouseStatus();
	bool GetCursorStatus();

private:

	// name of the system
	std::string systemName;

	// Pointer to window
	Window* window;

	// Pointer to camera
	Camera* camera;

	// Ply file loader
	cModelFileLoader* modelFileLoader;

	// Handler for all VAO related stuff
	cVAOManager* vaoManager;

	// Handler for all Texture related stuff
	cTextureManager* textureManager;

	// System for handling the GUI
	GUISystem* guiSystem;

	// Amount of ambient light
	float ambientLight;
};