#include "LightSystem.h"

#include "../OpenGL.h"
#include "../Components/LitComponent.h"
#include "../Components/ShaderComponent.h"

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

cLight* LightSystem::AddLight(glm::vec4 position)
{
	return lightManager->AddLight(position);
}

void LightSystem::GetAmbientLightAmount(float& amount)
{
	// lightManager->GetAmbientLightAmount(amount);
	amount = ambientLight;
}

void LightSystem::SetAmbientLightAmount(float amount)
{
	// lightManager->SetAmbientLightAmount(amount);
	ambientLight = amount;
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

			GLint isLitLocation = glGetUniformLocation(shaderComponent->shaderID, "isLit");

			shaderID = shaderComponent->shaderID;

			if (litComponent->isLit) {
				glUniform1f(isLitLocation, (GLfloat)GL_TRUE);
			}
			else {
				glUniform1f(isLitLocation, (GLfloat)GL_FALSE);
			}

			GLint ambientLightLocation = glGetUniformLocation(shaderID, "ambientLight");
			glUniform1f(ambientLightLocation, ambientLight);

			LoadLightUniformLocations(shaderComponent->shaderID);
			CopyLightInformationToShader(shaderComponent->shaderID);
		}
	}
}

void LightSystem::Shutdown() {

	lightManager = nullptr;
	delete lightManager;
}