#pragma once

#include <ECS/Component.h>

#include "../cVAOManager/cVAOManager.h"
#include "../cAnimationManager/cAnimator.h"

enum ModelFormat {
	PLY,
	FBX,
	OBJ
};

// Struct holding the actual mesh model which is to be drawn
struct MeshComponent : Component {
public:
	// Constructor
	MeshComponent() : 
		componentType("MeshComponent"),
		meshName(""),
		isWireframe(false),
		isSkyBox(false),
		isTerrain(false),
		isQuad(false),
		isFBO(false),
		isVisible(true),
		isImposter(false),
		useDiscardTexture(false),
		modelFormat(PLY),
		health(1000),
		useBones(false),
		useModelInfo(false),
		animation(nullptr),
		animator(nullptr)
	{};

	// Destructor
	~MeshComponent() {};

	// struct holding mesh name and vertex information
	sModelDrawInfo model;

	// name of the mesh
	std::string meshName;

	ModelFormat modelFormat;

	// Mesh is to be drawn in wireframe mode
	bool isWireframe;

	// Mesh is a skybox sphere
	bool isSkyBox;

	// Mesh is a terrain
	bool isTerrain;

	// If the mesh texture is coming from an FBO
	bool isFBO;

	// If the mesh is a screen quad
	bool isQuad;

	// If the mesh is visible
	bool isVisible;

	// If is an imposter
	bool isImposter;
	bool useDiscardTexture;

	bool useBones;
	bool useModelInfo;

	float health;

	Animator* animator = nullptr;
	Animation* animation = nullptr;

private:
	// name of the component
	std::string componentType;
};