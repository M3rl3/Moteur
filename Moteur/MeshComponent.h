#pragma once

#include <ECS/Component.h>

#include "cVAOManager/cVAOManager.h"

struct MeshComponent : Component {
public:
	MeshComponent() : 
		componentType("MeshComponent") {};
	~MeshComponent() {};

	std::string GetType() {
		return componentType;
	}

	std::string GetMeshName() {
		return plyModel.meshName;
	}

private:
	std::string componentType;

	sModelDrawInfo plyModel;
};