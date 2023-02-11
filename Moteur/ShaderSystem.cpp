#include "ShaderSystem.h"
#include "ShaderComponent.h"

#include <iostream>

ShaderSystem::ShaderSystem()
{
	shaderManager = new cShaderManager();
    shaderComponent = "ShaderComponent";
    meshComponent = "MeshComponent";
    transformComponent = "TransformComponent";
}

ShaderSystem::~ShaderSystem()
{

}

cShaderManager* ShaderSystem::GetShaderManager() {
	return shaderManager;
}


void ShaderSystem::Process(const std::vector<Entity*>& entities, float dt) {
    ShaderComponent* shaderComponent;

    for (int i = 0; i < entities.size(); i++) {

        Entity* entity = entities[i];
        shaderComponent = GetShaderComponent(entities[i]->GetComponents());

        
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

    shaderManager->useShaderProgram("ShadyProgram");
    id = shaderManager->getIDFromFriendlyName("ShadyProgram");
    //glUseProgram(id);

    //shaderID = id;
}

ShaderComponent* ShaderSystem::GetShaderComponent(const std::vector<Component*>& components)
{
    ShaderComponent* component = nullptr;
    for (int j = 0; j < components.size(); j++) {
        component = dynamic_cast<ShaderComponent*>(components[j]);

        if (component->GetType() == shaderComponent) {
            return component;
        }
    }
    return nullptr;
}

//TransfromComponent* ShaderSystem::GetTransformComponent(const std::vector<Component*>& components)
//{
//    TransfromComponent* component = nullptr;
//    for (int j = 0; j < components.size(); j++) {
//        component = dynamic_cast<TransfromComponent*>(components[j]);
//
//        if (component->GetType() == shaderComponent) {
//            return component;
//        }
//    }
//    return nullptr;
//}
