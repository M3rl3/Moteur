#pragma once

#include "ECSengine/ECSengine.h"

#include "Systems/ShaderSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/PhysicsSystem.h"
#include "Systems/MotionSystem.h"
#include "Systems/LightSystem.h"
#include "Systems/SoundSystem.h"
#include "Systems/AISystem.h"
#include "Systems/GUISystem.h"

#include "../Moteur/cAnimationManager/cAnimation.h"
#include "../Moteur/cAnimationManager/cAnimator.h"

#include "Timer/Timer.h"

enum GameMode {
	CAMERA,
	PLAYER,
	NONE
};

class Scene {
public:
	// Constructor
	Scene();
	// Destructor
	~Scene();

	// Lifecycle
	void Initialize();
	void Render();
	void Update(float dt);
	void Shutdown();

	// User defined update method
	static void UpdateCallback(float dt);
	
	// Utility
	void LoadModels();
	void LoadTextures();
	void LoadSounds();
	void LoadLighting();

	// Load the asset files
	void LoadAssets();

	// Create the floor
	void CreateFlatPlane();

	// Getters
	ECSengine* GetEngineInstance();
	float GetDeltaTime();

private:
	// Member Variables
	ECSengine* engine;
	Timer* timer;

	// Systems
	RenderSystem* renderSystem;
	PhysicsSystem* physicsSystem;
	ShaderSystem* shaderSystem;
	MotionSystem* motionSystem;
	LightSystem* lightSystem;
	SoundSystem* soundSystem;
	AISystem* aiSystem;

	unsigned int shaderID = 0;

	float enemyHealth = 0;
};