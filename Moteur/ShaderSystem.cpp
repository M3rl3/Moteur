#include "ShaderSystem.h"
#include "ShaderComponent.h"
#include "MeshComponent.h"

#include "OpenGL.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

// Constructor
ShaderSystem::ShaderSystem()
{
	shaderManager = new cShaderManager();
}

// Destructor
ShaderSystem::~ShaderSystem()
{

}

// getter for shader manager
cShaderManager* ShaderSystem::GetShaderManager() 
{
	return shaderManager;
}

// Update method of the system, called every tick
void ShaderSystem::Process(const std::vector<Entity*>& entities, float dt) {

    ShaderComponent* shaderComponent = nullptr;
    MeshComponent* meshComponent = nullptr;
    // TransformComponent* transformComponent;
    
    // Iterate vec entities
    for (int i = 0; i < entities.size(); i++) {
        Entity* currentEntity = entities[i];

        shaderComponent = currentEntity->GetComponentByType<ShaderComponent>();
        meshComponent = currentEntity->GetComponentByType<MeshComponent>();
        // transformComponent = currentEntity->GetComponentByType<TransformComponent>();

        if ( meshComponent != nullptr && shaderComponent != nullptr) {

            GLint useIsTerrainMeshLocation = glGetUniformLocation(shaderComponent->shaderID, "bIsTerrainMesh");

            // Check if object is a terrain mesh
            if (meshComponent->isTerrain) {
                glUniform1f(useIsTerrainMeshLocation, (GLfloat)GL_TRUE);
            }
            else {
                glUniform1f(useIsTerrainMeshLocation, (GLfloat)GL_FALSE);
            }

            GLint bIsSkyboxObjectLocation = glGetUniformLocation(shaderComponent->shaderID, "bIsSkyboxObject");

            // Check if object is a skybox mesh
            if (meshComponent->isSkyBox) {
                glUniform1f(bIsSkyboxObjectLocation, (GLfloat)GL_TRUE);
            }
            else {
                glUniform1f(bIsSkyboxObjectLocation, (GLfloat)GL_FALSE);
            }
        }
    }
}

// Compile the shaders provided
void ShaderSystem::CreateShaderProgramFromFiles(unsigned int& id, const char* vertShader, const char* fragShader)
{
	cShaderManager::cShader vertexShader;
	cShaderManager::cShader fragmentShader;

    // path to the shaders
    vertexShader.fileName = vertShader;
    fragmentShader.fileName = fragShader;

    // Check if the shaders compile
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

// Gracefully closes everything down

void ShaderSystem::Shutdown()
{
    shaderManager = nullptr;
    delete shaderManager;
}
