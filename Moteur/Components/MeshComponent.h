#pragma once

#include <ECS/Component.h>

#include "../cVAOManager/cVAOManager.h"

enum ModelType {
	PLY,
	FBX
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
		modelType(PLY)
	{};

	// Destructor
	~MeshComponent() {};

	// struct holding mesh name and vertex information
	sModelDrawInfo plyModel;

	// name of the mesh
	std::string meshName;

	ModelType modelType;

	// Mesh is to be drawn in wireframe mode
	bool isWireframe;

	// Mesh is a skybox sphere
	bool isSkyBox;

	// Mesh is a terrain
	bool isTerrain;

private:
	// name of the component
	std::string componentType;
};