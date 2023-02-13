#pragma once

#include <ECS/Component.h>

#include "cVAOManager/cVAOManager.h"

struct MeshComponent : Component {
public:
	MeshComponent() : 
		componentType("MeshComponent"),
		isSkyBox(false),
		isTerrain(false),
		doNotLight(false)
	{};
	~MeshComponent() {};

	sModelDrawInfo plyModel;

	bool isSkyBox;
	bool isTerrain;
	bool doNotLight;

private:
	std::string componentType;
};