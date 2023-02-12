#pragma once

#include <ECS/Component.h>

#include "cVAOManager/cVAOManager.h"

struct MeshComponent : Component {
public:
	MeshComponent() : 
		componentType("MeshComponent") {};
	~MeshComponent() {};

	sModelDrawInfo plyModel;

private:
	std::string componentType;
};