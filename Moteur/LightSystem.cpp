#include "LightSystem.h"

#include "OpenGL.h"
#include "LitComponent.h"
#include "ShaderComponent.h"

LightSystem::LightSystem()
{
	lightManager = new cLightManager();
}

LightSystem::~LightSystem()
{
}

cLightManager* LightSystem::GetLightManager() {
	return lightManager;
}

cLight* LightSystem::AddLight()
{
	return lightManager->AddLight();
}

void LightSystem::CopyLightInformationToShader(unsigned int shaderID) {
	lightManager->CopyLightInformationToShader(shaderID);
}

void LightSystem::LoadLightUniformLocations(unsigned int shaderID) {
	lightManager->LoadLightUniformLocations(shaderID);
}

void LightSystem::Process(const std::vector<Entity*>& entities, float dt) {

	ShaderComponent* shaderComponent = nullptr;
	LitComponent* litComponent = nullptr;

	for (int i = 0; i < entities.size(); i++) {

		Entity* currentEntity = entities[i];

		shaderComponent = currentEntity->GetComponentByType<ShaderComponent>();
		litComponent = currentEntity->GetComponentByType<LitComponent>();

		if (litComponent != nullptr && shaderComponent != nullptr) {

			GLint doNotLightLocation = glGetUniformLocation(shaderComponent->shaderID, "doNotLight");

			if (!litComponent->doNotLight) {
				glUniform1f(doNotLightLocation, (GLfloat)GL_FALSE);

				LoadLightUniformLocations(shaderComponent->shaderID);
				CopyLightInformationToShader(shaderComponent->shaderID);
			}
			else {
				glUniform1f(doNotLightLocation, (GLfloat)GL_TRUE);
			}
		}
	}
}

void LightSystem::Shutdown() {

	lightManager = nullptr;
	delete lightManager;
}