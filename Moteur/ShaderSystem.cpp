#include "ShaderSystem.h"
#include "ShaderComponent.h"
#include "MeshComponent.h"

#include "OpenGL.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

ShaderSystem::ShaderSystem()
{
	shaderManager = new cShaderManager();
}

ShaderSystem::~ShaderSystem()
{

}

cShaderManager* ShaderSystem::GetShaderManager() 
{
	return shaderManager;
}

void ShaderSystem::Process(const std::vector<Entity*>& entities, float dt) {
    ShaderComponent* shaderComponent;
    MeshComponent* meshComponent;
    TransformComponent* transformComponent;
    
    for (int i = 0; i < entities.size(); i++) {
        Entity* currentEntity = entities[i];

        shaderComponent = currentEntity->GetComponentByType<ShaderComponent>();
        meshComponent = currentEntity->GetComponentByType<MeshComponent>();
        transformComponent = currentEntity->GetComponentByType<TransformComponent>();

        if ( meshComponent != nullptr && shaderComponent != nullptr) {

            GLint doNotLightLocation = glGetUniformLocation(shaderComponent->shaderID, "doNotLight");
            if (meshComponent->doNotLight) {
                glUniform1f(doNotLightLocation, (GLfloat)GL_TRUE);
            }
            else {
                glUniform1f(doNotLightLocation, (GLfloat)GL_FALSE);
            }

            GLint useIsTerrainMeshLocation = glGetUniformLocation(shaderComponent->shaderID, "bIsTerrainMesh");

            if (meshComponent->isTerrain) {
                glUniform1f(useIsTerrainMeshLocation, (GLfloat)GL_TRUE);
            }
            else {
                glUniform1f(useIsTerrainMeshLocation, (GLfloat)GL_FALSE);
            }

            GLint bIsSkyboxObjectLocation = glGetUniformLocation(shaderComponent->shaderID, "bIsSkyboxObject");

            if (meshComponent->isSkyBox) {
                glUniform1f(bIsSkyboxObjectLocation, (GLfloat)GL_TRUE);
            }
            else {
                glUniform1f(bIsSkyboxObjectLocation, (GLfloat)GL_FALSE);
            }
        }
    }
}

void ShaderSystem::CreateShaderProgramFromFiles(unsigned int& id, const char* vertShader, const char* fragShader)
{
	cShaderManager::cShader vertexShader;
	cShaderManager::cShader fragmentShader;

    vertexShader.fileName = vertShader;
    fragmentShader.fileName = fragShader;

    if (!shaderManager->createProgramFromFile("ShadyProgram", vertexShader, fragmentShader)) {
        std::cout << "Error: Shader program failed to compile." << std::endl;
        std::cout << shaderManager->getLastError();
        return;
    }
    else {
        std::cout << "Shaders compiled." << std::endl;
    }

    //shaderManager->useShaderProgram("ShadyProgram");
    id = shaderManager->getIDFromFriendlyName("ShadyProgram");
    glUseProgram(id);

    shaderID = id;
}

void ShaderSystem::Shutdown()
{
    shaderManager = nullptr;
    delete shaderManager;
}
