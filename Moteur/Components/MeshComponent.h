#pragma once

#include <ECS/Component.h>

#include "../cVAOManager/cVAOManager.h"

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
		modelFormat(PLY)
	{};

	// Destructor
	~MeshComponent() {};

	// struct holding mesh name and vertex information
	sModelDrawInfo plyModel;

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

private:
	// name of the component
	std::string componentType;
};