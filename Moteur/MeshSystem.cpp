#include "MeshSystem.h"

#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "TextureComponent.h"

#include "OpenGL.h"

MeshSystem::MeshSystem()
{
	systemName = "MeshSystem";
	vaoManager = new cVAOManager();
	textureManager = new cBasicTextureManager();
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

void MeshSystem::SetTexturePath(const char* filePath)
{
    std::cout << "\nLoading Textures";

    textureManager->SetBasePath(filePath);
}

bool MeshSystem::Create2DTextureFromBMPFile(unsigned int& textureID, const char* filePath)
{
    if (textureManager->Create2DTextureFromBMPFile(filePath))
    {
        textureID = textureManager->getTextureIDFromName(filePath);

        std::cout << "Loaded " << filePath << " texture." << std::endl;
		return true;
    }
    else
    {
        std::cout << "Error: failed to load " << filePath << " texture." << std::endl;
		return false;
    }
}

bool MeshSystem::CreateCubeMapTextureFromBMPFiles(
    unsigned int& textureID,
    std::string cubeMapName,
    std::string posX_fileName, std::string negX_fileName,
    std::string posY_fileName, std::string negY_fileName,
    std::string posZ_fileName, std::string negZ_fileName,
    bool bIsSeamless, std::string& errorString)
{
    const char* skybox_name = cubeMapName.c_str();
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

void MeshSystem::Process(const std::vector<Entity*>& entities, float dt)
{
	ShaderComponent* shaderComponent;
    TextureComponent* textureComponent;

	for (int i = 0; i < entities.size(); i++) {
		Entity* currentEntity = entities[i];

		shaderComponent = currentEntity->GetComponentByType<ShaderComponent>();
		textureComponent = currentEntity->GetComponentByType<TextureComponent>();

        if (shaderComponent != nullptr && textureComponent != nullptr) {

            GLint useRGBAColourLocation = glGetUniformLocation(shaderComponent->shaderID, "useRGBAColour");
            GLint RGBAColourLocation = glGetUniformLocation(shaderComponent->shaderID, "RGBAColour");
            GLint hasTextureLocation = glGetUniformLocation(shaderComponent->shaderID, "bHasTexture");

            if (textureComponent->useRGBAColor) {

                glUniform1f(useRGBAColourLocation, (GLfloat)GL_TRUE);

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

