#pragma once

#include <ECS/Component.h>

#include "cVAOManager/cVAOManager.h"

class MeshComponent : Component {
public:
	MeshComponent() : componentType("MeshComponent") {};
	virtual ~MeshComponent() = 0;

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