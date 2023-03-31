#include "MeshSystem.h"

#include "../Components/MeshComponent.h"
#include "../Components/ShaderComponent.h"
#include "../Components/TextureComponent.h"

#include "../OpenGL.h"

// Constructor
MeshSystem::MeshSystem()
{
	systemName = "MeshSystem";
	vaoManager = new cVAOManager();
	textureManager = new cTextureManager();
}

// Destructor
MeshSystem::~MeshSystem() 
{

}

// Getter for the VAO manager object
cVAOManager* MeshSystem::GetVAOManager() 
{
	return vaoManager;
}

// Load the mesh model from ply file into VAO
bool MeshSystem::LoadMesh(std::string fileName, std::string modelName, sModelDrawInfo& plyModel, unsigned int shaderID)
{
	LoadModel(fileName, plyModel);

    // Check if model loaded
	if (vaoManager->LoadModelIntoVAO(modelName, plyModel, shaderID)) {
		std::cout << "Model " << modelName << " loaded successfully." << std::endl;
		return true;
	}
	else {
		std::cout << "Could not load model " << modelName << " into VAO" << std::endl;
		return false;
	}
}

// Set the path where the textures are located
void MeshSystem::SetTexturePath(const char* filePath)
{
    std::cout << "\nLoading Textures...";

    textureManager->SetBasePath(filePath);
}

// Load a 2D texture
bool MeshSystem::Load2DTexture(unsigned int& textureID, const char* filePath)
{
    // Check if the texture loaded
    if (textureManager->Create2DTextureFromBMPFile(filePath))
    {
        textureID = textureManager->getTextureIDFromName(filePath);

        std::cout << "\nLoaded " << filePath << " texture." << std::endl;
		return true;
    }
    else
    {
        std::cout << "Error: failed to load " << filePath << " texture." << std::endl;
		return false;
    }
}

// Load a skybox texture
bool MeshSystem::LoadCubeMapTexture(
    unsigned int& textureID,
    std::string cubeMapName,
    std::string posX_fileName, std::string negX_fileName,
    std::string posY_fileName, std::string negY_fileName,
    std::string posZ_fileName, std::string negZ_fileName,
    bool bIsSeamless, std::string& errorString)
{
    const char* skybox_name = cubeMapName.c_str();

    // Check if the skybox loaded
    if (textureManager->CreateCubeTextureFromBMPFiles(skybox_name,
        posX_fileName, negX_fileName,
        posY_fileName, negY_fileName,
        posZ_fileName, negZ_fileName,
        bIsSeamless, errorString))
    {
        textureID = textureManager->getTextureIDFromName(cubeMapName);
        std::cout << "\nLoaded skybox textures: " << skybox_name << std::endl;
        return true;
    }
    else
    {
        std::cout << "\nError: failed to load skybox because " << errorString;
        return false;
    }
}

// The update method called every tick
void MeshSystem::Process(const std::vector<Entity*>& entities, float dt)
{
	ShaderComponent* shaderComponent = nullptr;
    TextureComponent* textureComponent = nullptr;

    // Iterate through vec entities
	for (int i = 0; i < entities.size(); i++) {
		Entity* currentEntity = entities[i];

		shaderComponent = currentEntity->GetComponentByType<ShaderComponent>();
		textureComponent = currentEntity->GetComponentByType<TextureComponent>();

        // Check if entity has the components needed
        if (shaderComponent != nullptr && textureComponent != nullptr) {

            GLint useRGBAColourLocation = glGetUniformLocation(shaderComponent->shaderID, "useRGBAColour");
            GLint RGBAColourLocation = glGetUniformLocation(shaderComponent->shaderID, "RGBAColour");
            GLint hasTextureLocation = glGetUniformLocation(shaderComponent->shaderID, "bHasTexture");

            // Check if it uses RGBA color or a texture
            if (textureComponent->useRGBAColor) {

                glUniform1f(useRGBAColourLocation, (GLfloat)GL_TRUE);
                glUniform1f(hasTextureLocation, (GLfloat)GL_FALSE);

                glm::vec4 color = textureComponent->rgbaColor;

                glUniform4f(RGBAColourLocation, color.r, color.g, color.b, color.w);
            }
            else {
                glUniform1f(useRGBAColourLocation, (GLfloat)GL_FALSE);   
                glUniform1f(hasTextureLocation, (GLfloat)GL_TRUE);

                std::string texture0 = textureComponent->textures[0];

                GLuint texture0ID = textureManager->getTextureIDFromName(texture0);

                GLuint texture0Unit = 0;
                glActiveTexture(texture0Unit + GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture0ID);

                // set uniform locations
                GLint texture0Location = glGetUniformLocation(shaderComponent->shaderID, "texture0");
                glUniform1i(texture0Location, texture0Unit);

                GLint texRatio_0_3 = glGetUniformLocation(shaderComponent->shaderID, "texRatio_0_3");
                glUniform4f(texRatio_0_3,
                    textureComponent->textureRatios[0],
                    textureComponent->textureRatios[1],
                    textureComponent->textureRatios[2],
                    textureComponent->textureRatios[3]);
            }
        }
	}
}

// Gracefully closes everything down
void MeshSystem::Shutdown() {

    vaoManager = nullptr;
    delete vaoManager;

    textureManager = nullptr;
    delete textureManager;
}