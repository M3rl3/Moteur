#include "MeshSystem.h"

#include "MeshComponent.h"
#include "ShaderComponent.h"

#include "OpenGL.h"

MeshSystem::MeshSystem()
{
	systemName = "MeshSystem";
	vaoManager = new cVAOManager();
}

MeshSystem::~MeshSystem() 
{

}

cVAOManager* MeshSystem::GetVAOManager() 
{
	return vaoManager;
}

bool MeshSystem::LoadMesh(std::string fileName, std::string modelName, sModelDrawInfo& plyModel, unsigned int shaderID)
{
	LoadModel(fileName, plyModel);

	if (vaoManager->LoadModelIntoVAO(modelName, plyModel, shaderID)) {
		std::cout << "Model " << modelName << " loaded successfully." << std::endl;
		return true;
	}
	else {
		std::cout << "Could not load model " << modelName << " into VAO" << std::endl;
		return false;
	}
}

void MeshSystem::Process(const std::vector<Entity*>& entities, float dt)
{
	/*ShaderComponent* shaderComponent;
	MeshComponent* meshComponent;

	for (int i = 0; i < entities.size(); i++) {
		Entity* currentEntity = entities[i];

		shaderComponent = currentEntity->GetComponentByType<ShaderComponent>();
		meshComponent = currentEntity->GetComponentByType<MeshComponent>();

		std::string meshName = meshComponent->plyModel.meshName;

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		sModelDrawInfo modelInfo;
		if (vaoManager->FindDrawInfoByModelName(meshName, modelInfo)) {

			glBindVertexArray(modelInfo.VAO_ID);
			glDrawElements(GL_TRIANGLES, modelInfo.numberOfIndices, GL_UNSIGNED_INT, (void*)0);
			glBindVertexArray(0);
		}
		else {
			std::cout << "Model " << meshName << " not found in VAO." << std::endl;
		}
	}*/
}

