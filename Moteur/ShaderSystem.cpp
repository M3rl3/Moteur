#include "ShaderSystem.h"
#include "ShaderComponent.h"

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

cShaderManager* ShaderSystem::GetShaderManager() {
	return shaderManager;
}

void ShaderSystem::Process(const std::vector<Entity*>& entities, float dt) {
   /* ShaderComponent* shaderComponent;
    
    for (int i = 0; i < entities.size(); i++) {
        Entity* currentEntity = entities[i];
        shaderComponent = dynamic_cast<ShaderComponent*>(currentEntity->GetComponentByType("ShaderComponent"));
        if (shaderComponent != nullptr) {
            shaderComponent->shaderID = shaderID;
        }
    }*/
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
