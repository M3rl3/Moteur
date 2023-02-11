#include "ShaderSystem.h"

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

    shaderManager->useShaderProgram("ShadyProgram");
    id = shaderManager->getIDFromFriendlyName("ShadyProgram");
    //glUseProgram(id);

    //shaderID = id;
}
