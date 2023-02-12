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
    ShaderComponent* shaderComponent;
    
    for (int i = 0; i < entities.size(); i++) {

        if (shaderComponent != nullptr) {
            shaderComponent->shaderID = shaderID;
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

Camera* ShaderSystem::GetCamera()
{
    return camera;
}

void ShaderSystem::SetCameraPosition(glm::vec3 cameraEye)
{
    camera->position = cameraEye;
}

void ShaderSystem::SetCameraTarget(glm::vec3 cameraTarget)
{
    camera->target = cameraTarget;
}

ShaderComponent* ShaderSystem::GetShaderComponent(const std::vector<Component*>& components)
{
    Component* component = nullptr;
    for (int j = 0; j < components.size(); j++) {

        component = components[j];
        if (component->GetType() == shaderComponent) {
            return dynamic_cast<ShaderComponent*>(component);
        }
    }
    return nullptr;
}

//TransformComponent* ShaderSystem::GetTransformComponent(const std::vector<Component*>& components)
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
