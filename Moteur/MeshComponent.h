#pragma once

#include <ECS/Component.h>

#include "cVAOManager/cVAOManager.h"

// Struct holding the actual mesh model which is to be drawn
struct MeshComponent : Component {
public:
	// Constructor
	MeshComponent() : 
		componentType("MeshComponent"),
		isSkyBox(false),
		isTerrain(false)
	{};

	// Destructor
	~MeshComponent() {};

	// The mesh object containing vertices and indices
	sModelDrawInfo plyModel;

	// Mesh is a skybox sphere
	bool isSkyBox;

	// Mesh is a terrain
	bool isTerrain;

private:
	// name of the component
	std::string componentType;
};