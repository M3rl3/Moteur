#include "cModelFileLoader.h"
#include "../cAnimationManager/AssimpGLMHelpers.h"
#include "../Global.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

Assimp::Importer assimpImporter;
cTextureManager* textureMan;

struct vertexLayout {

    float x, y, z;
    float nx, ny, nz;
    float r, g, b, a;
    float texture_u, texture_v;
    float BoneID[4];
    float BoneWeight[4];
};

struct triangleLayout {

    unsigned int triangleIndices[3];
};

void CastToGLM(const aiMatrix4x4& from, glm::mat4& to)
{
    to = glm::transpose(glm::make_mat4(&from.a1));

    //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
    /*to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
    to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
    to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
    to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;*/
}

void CastToGLM(const aiQuaternion& in, glm::quat& out)
{
    out.w = in.w;
    out.x = in.x;
    out.y = in.y;
    out.z = in.z;
}

void CastToGLM(const aiVector3D& in, glm::vec3& out)
{
    out.x = in.x;
    out.y = in.y;
    out.z = in.z;
}

cModelFileLoader::cModelFileLoader() {
    textureMan = new cTextureManager();
}

cModelFileLoader::~cModelFileLoader() {

}

sModelDrawInfo* cModelFileLoader::GetPlyModelByID(unsigned int id) {

    return plyModels[id];
}

void cModelFileLoader::SetBasePath(std::string filePath)
{
    basePath = filePath;
}

int cModelFileLoader::LoadModelPLY(std::string fileName, sModelDrawInfo& plyModel) {

    vertexLayout* modelArray = NULL;
    triangleLayout* triangleArray = NULL;

    std::string fileToLoadFullPath = this->basePath + "/" + fileName;

    std::ifstream plyFile(fileToLoadFullPath);
    if (!plyFile.is_open()) {
        std::cout << "Could not load model file " << fileName << std::endl;
        return -1;
    }

    std::string input1;

    // Scan for the word "vertex"
    while (plyFile >> input1)
    {
        if (input1 == "vertex")
        {
            break;
        }
    }

    plyFile >> plyModel.numberOfVertices;

    // Scan for the word "face"
    while (plyFile >> input1)
    {
        if (input1 == "face")
        {
            break;
        }
    }

    plyFile >> plyModel.numberOfTriangles;

    // Scan for the word "end_header"
    while (plyFile >> input1)
    {
        if (input1 == "end_header")
        {
            break;
        }
    }

    //Vertex Layout
    modelArray = new vertexLayout[plyModel.numberOfVertices];

    std::cout << "Loading " << fileName << std::endl;
    for (unsigned int count = 0; count != plyModel.numberOfVertices; count++)
    {
        plyFile >> modelArray[count].x;
        plyFile >> modelArray[count].y;
        plyFile >> modelArray[count].z;

        plyFile >> modelArray[count].nx;
        plyFile >> modelArray[count].ny;
        plyFile >> modelArray[count].nz;

        plyFile >> modelArray[count].r;
        plyFile >> modelArray[count].g;
        plyFile >> modelArray[count].b;
        plyFile >> modelArray[count].a;

        plyFile >> modelArray[count].texture_u;
        plyFile >> modelArray[count].texture_v;
    }

    triangleArray = new triangleLayout[plyModel.numberOfTriangles];

    // Triangle Layout
    for (unsigned int count = 0; count != plyModel.numberOfTriangles; count++) {
        unsigned int temp = 0;
        plyFile >> temp;

        plyFile >> triangleArray[count].triangleIndices[0];
        plyFile >> triangleArray[count].triangleIndices[1];
        plyFile >> triangleArray[count].triangleIndices[2];
    }
    plyFile.close();

    plyModel.pVertices = new vertLayout[plyModel.numberOfVertices];

    for (unsigned int index = 0; index != plyModel.numberOfVertices; index++)
    {
        plyModel.pVertices[index].x = modelArray[index].x;
        plyModel.pVertices[index].y = modelArray[index].y;
        plyModel.pVertices[index].z = modelArray[index].z;

        plyModel.pVertices[index].r = modelArray[index].r;
        plyModel.pVertices[index].g = modelArray[index].g;
        plyModel.pVertices[index].b = modelArray[index].b;
        plyModel.pVertices[index].a = modelArray[index].a;

        plyModel.pVertices[index].nx = modelArray[index].nx;
        plyModel.pVertices[index].ny = modelArray[index].ny;
        plyModel.pVertices[index].nz = modelArray[index].nz;

        plyModel.pVertices[index].u0 = modelArray[index].texture_u;
        plyModel.pVertices[index].v0 = modelArray[index].texture_v;
    }

    plyModel.numberOfIndices = plyModel.numberOfTriangles * 3;
    plyModel.pIndices = new unsigned int[plyModel.numberOfIndices];

    unsigned int indexIndices = 0;

    for (unsigned int triangleIndex = 0; triangleIndex != plyModel.numberOfTriangles; triangleIndex++)
    {
        plyModel.pIndices[indexIndices + 0] = triangleArray[triangleIndex].triangleIndices[0];
        plyModel.pIndices[indexIndices + 1] = triangleArray[triangleIndex].triangleIndices[1];
        plyModel.pIndices[indexIndices + 2] = triangleArray[triangleIndex].triangleIndices[2];

        indexIndices += 3;
    }

    delete[] modelArray;
    delete[] triangleArray;

    // Store the model and return its index
    if (&plyModel != nullptr) {
        plyModels.push_back(&plyModel);
        return plyModels.size() - 1;
    }
    else {
        return -1;
    }
}

int cModelFileLoader::LoadModelFBX(std::string fileName, sModelDrawInfo& fbxModel)
{
    std::string fileToLoadFullPath = this->basePath + "/" + fileName;

    const aiScene* scene = assimpImporter.ReadFile(fileToLoadFullPath,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_FixInfacingNormals |
        aiProcess_LimitBoneWeights |
        aiProcess_ConvertToLeftHanded);

    if (!scene) {
        std::cout << "Could not load model file " << fileName << std::endl;
        return -1;
    }

    std::cout << "Loading " << fileName << std::endl;

    if (scene->HasTextures())
    {
        aiTexture* texture = scene->mTextures[0];   // Get the first texture in the scene
        unsigned char* buffer = LoadEmbeddedTexture(texture);

        std::string errorString = "";
        if (!textureMan->Create2DTextureFromLocalBuffer(buffer, fileName, fileToLoadFullPath, errorString)) {
            std::cout << "\n Error: could not load embedded texture." << std::endl;
        }
    }

    // Process all nodes in the scene
    ProcessNode(scene->mRootNode, scene, fbxModel);

    // Store the model and return its index
    if (&fbxModel != nullptr) {
        fbxModels.push_back(&fbxModel);
        return fbxModels.size() - 1;
    }
    else {
        return -1;
    }
}

int cModelFileLoader::TransferColorTextureToVertex(std::string fileName)
{
    // Load the FBX file into an Assimp scene
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(fileName, aiProcessPreset_TargetRealtime_MaxQuality);

    // Iterate through the meshes of the loaded scene
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[i];
        aiVector3D* texCoords = mesh->mTextureCoords[0];
        aiColor4D* colors = new aiColor4D[mesh->mNumVertices];

        // Iterate through the vertices of the mesh and calculate the color for each vertex
        for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
            aiVector3D texCoord = texCoords[j];
            aiColor4D color;
            // Calculate color based on texture color and assign it to the color array
            // For example, you can use the texture color multiplied by a constant factor
            color.r = mesh->mColors[0][j].r * texCoord.x;
            color.g = mesh->mColors[0][j].g * texCoord.y;
            color.b = mesh->mColors[0][j].b * texCoord.z;
            color.a = mesh->mColors[0][j].a;
            colors[j] = color;
        }

        // Set the color array as the vertex color of the mesh
        mesh->mColors[0] = colors;
        // mesh->mNumColorChannels = 1;
    }

    // Save the modified scene back to an FBX file
    // importer.Export(scene, "output.fbx", "fbx");

    // Clean up
    delete scene;

    return 0;
}

unsigned char* cModelFileLoader::LoadEmbeddedTexture(aiTexture* texture)
{
    // Get the texture data as a byte array
    const aiTexel* texel = texture->pcData;
    size_t size = texture->mWidth * texture->mHeight * sizeof(aiTexel);

    const unsigned char* texels = reinterpret_cast<const unsigned char*>(texel);
    int* width = reinterpret_cast<int*>(texture->mWidth);
    int* height = reinterpret_cast<int*>(texture->mHeight);

    int* channels;
    int jpg = 3;    // rgb
    int png = 4;    // rgba

    if (texture->CheckFormat("png")) {
        channels = &png;
    }
    else if (texture->CheckFormat("jpg")) {
        channels = &jpg;
    }
    else {
        // Has to have atleast 3 channels
        channels = &jpg;
    }  

    // Decode the texture using stb_image
    unsigned char* data = stbi_load_from_memory(texels, size, width, height, channels, STBI_rgb_alpha);

    // Return the texture data
    return data;
}

void extractColorFromTexture(aiTexture* texture, float u, float v, aiColor4D& color) {

    // Get texture width and height
    int width = texture->mWidth;
    int height = texture->mHeight;

    // Compute texture coordinates
    int x = static_cast<int>(u * width);
    int y = static_cast<int>(v * height);

    // Get texel at the specified coordinates
    aiTexel texel = texture->pcData[y * width + x];

    // Convert texel color to aiColor4D
    color.r = static_cast<float>(texel.b) / 255.0f;
    color.g = static_cast<float>(texel.g) / 255.0f;
    color.b = static_cast<float>(texel.r) / 255.0f;
    color.a = static_cast<float>(texel.a) / 255.0f;
}

void applyTextureToVertexColors(aiMesh * mesh, aiTexture * texture) {

    // Iterate over all vertices in the mesh
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {

        aiVector3D& vertex = mesh->mVertices[i];
        aiVector3D& normal = mesh->mNormals[i];
        aiVector3D& tangent = mesh->mTangents[i];
        aiVector3D& bitangent = mesh->mBitangents[i];
        aiColor4D& color = mesh->mColors[0][i];
        aiVector3D& texcoord = mesh->mTextureCoords[0][i];

        // Compute texture coordinates in the range [0, 1]
        float u = fmod(texcoord.x, 1.0f);
        float v = fmod(texcoord.y, 1.0f);

        // Extract color from texture
        aiColor4D texelColor;
        extractColorFromTexture(texture, u, v, texelColor);

        // Apply color to vertex color
        color.r = texelColor.r;
        color.g = texelColor.g;
        color.b = texelColor.b;
        color.a = 1.0f;
    }
}

void SetVertexBoneDataToDefault(vertLayout* vertex)
{
    for (int i = 0; i < MAX_BONE_WEIGHTS; i++)
    {
        vertex->BoneID[i] = -1;
        vertex->BoneWeight[i] = 0.0f;
    }
}

void SetVertexBoneData(vertLayout* vertex, int boneID, float weight)
{
    for (int i = 0; i < MAX_BONE_WEIGHTS; ++i)
    {
        if (vertex->BoneID[i] < 0)
        {
            vertex->BoneWeight[i] = weight;
            vertex->BoneID[i] = boneID;
            break;
        }
    }
}

unsigned int GetBoneIDFromName(const std::string& boneName, const std::vector<aiBone*>& boneArray)
{
    for (unsigned int i = 0; i < boneArray.size(); i++)
    {
        aiBone* bone = boneArray[i];
        if (bone->mName.data == boneName)
        {
            return i;
        }
    }
    return 0;
}

void cModelFileLoader::ProcessNode(aiNode* node, const aiScene* scene, sModelDrawInfo& fbxModel)
{
    vertLayout* modelArray = NULL;

    // Process all meshes in this node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {

        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        fbxModel.numberOfVertices = mesh->mNumVertices;
        fbxModel.numberOfTriangles = mesh->mNumFaces;
        fbxModel.numberOfIndices = mesh->mNumFaces * 3;

        modelArray = new vertLayout[fbxModel.numberOfVertices];

        for (unsigned int i = 0; i != fbxModel.numberOfVertices; i++) {

            SetVertexBoneDataToDefault(&modelArray[i]);

            modelArray[i].x = mesh->mVertices[i].x;
            modelArray[i].y = mesh->mVertices[i].y;
            modelArray[i].z = mesh->mVertices[i].z;
            modelArray[i].w = 1.f;

            modelArray[i].nx = mesh->mNormals[i].x;
            modelArray[i].ny = mesh->mNormals[i].y;
            modelArray[i].nz = mesh->mNormals[i].z;
            modelArray[i].nw = 0.f;

            if (mesh->HasVertexColors(0)) {

                modelArray[i].r = mesh->mColors[0][i].r;
                modelArray[i].g = mesh->mColors[0][i].g;
                modelArray[i].b = mesh->mColors[0][i].b;
                modelArray[i].a = mesh->mColors[0][i].a;
            }
            else {

                modelArray[i].r = 0.f;
                modelArray[i].g = 0.f;
                modelArray[i].b = 0.f;
                modelArray[i].a = 0.f;
            }

            if (mesh->HasTextureCoords(0)) {

                modelArray[i].u0 = mesh->mTextureCoords[0][i].x;
                modelArray[i].v0 = mesh->mTextureCoords[0][i].y;
                modelArray[i].u1 = mesh->mTextureCoords[0][i].z;
                modelArray[i].v1 = 0.f;
            }
            else {

                modelArray[i].u0 = 0.f;
                modelArray[i].v0 = 0.f;
                modelArray[i].u1 = 0.f;
                modelArray[i].v1 = 0.f;
            }

            fbxModel.numberOfBones = mesh->mNumBones;
            for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
            {
                int boneID = -1;
                std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
                if (fbxModel.m_BoneInfoMap.find(boneName) == fbxModel.m_BoneInfoMap.end())
                {
                    BonesInfo newBoneInfo;
                    newBoneInfo.id = fbxModel.m_BoneCounter;
                    CastToGLM(
                        mesh->mBones[boneIndex]->mOffsetMatrix, newBoneInfo.offset);
                    fbxModel.m_BoneInfoMap[boneName] = newBoneInfo;
                    boneID = fbxModel.m_BoneCounter;
                    fbxModel.m_BoneCounter++;
                }
                else
                {
                    boneID = fbxModel.m_BoneInfoMap[boneName].id;
                }

                assert(boneID != -1);
                auto weights = mesh->mBones[boneIndex]->mWeights;
                int numWeights = mesh->mBones[boneIndex]->mNumWeights;

                for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
                {
                    int vertexId = weights[weightIndex].mVertexId;
                    float weight = weights[weightIndex].mWeight;
                    assert(vertexId <= fbxModel.numberOfVertices);
                    SetVertexBoneData(&modelArray[vertexId], boneID, weight);
                }
            }
        }

        // Copy the vertex data
        fbxModel.pVertices = new vertLayout[fbxModel.numberOfVertices];

        for (unsigned int index = 0; index != fbxModel.numberOfVertices; index++)
        {
            fbxModel.pVertices[index].x = modelArray[index].x;
            fbxModel.pVertices[index].y = modelArray[index].y;
            fbxModel.pVertices[index].z = modelArray[index].z;
            fbxModel.pVertices[index].w = modelArray[index].w;

            fbxModel.pVertices[index].r = modelArray[index].r;
            fbxModel.pVertices[index].g = modelArray[index].g;
            fbxModel.pVertices[index].b = modelArray[index].b;
            fbxModel.pVertices[index].a = modelArray[index].a;

            fbxModel.pVertices[index].nx = modelArray[index].nx;
            fbxModel.pVertices[index].ny = modelArray[index].ny;
            fbxModel.pVertices[index].nz = modelArray[index].nz;
            fbxModel.pVertices[index].nw = modelArray[index].nw;

            fbxModel.pVertices[index].u0 = modelArray[index].u0;
            fbxModel.pVertices[index].v0 = modelArray[index].v0;
            fbxModel.pVertices[index].u1 = modelArray[index].u1;
            fbxModel.pVertices[index].v1 = modelArray[index].v1;

            fbxModel.pVertices[index].BoneID[0] = modelArray[index].BoneID[0];
            fbxModel.pVertices[index].BoneID[1] = modelArray[index].BoneID[1];
            fbxModel.pVertices[index].BoneID[2] = modelArray[index].BoneID[2];
            fbxModel.pVertices[index].BoneID[3] = modelArray[index].BoneID[3];
                
            fbxModel.pVertices[index].BoneWeight[0] = modelArray[index].BoneWeight[0];
            fbxModel.pVertices[index].BoneWeight[1] = modelArray[index].BoneWeight[1];
            fbxModel.pVertices[index].BoneWeight[2] = modelArray[index].BoneWeight[2];
            fbxModel.pVertices[index].BoneWeight[3] = modelArray[index].BoneWeight[3];
        }

        // Copy the index data
        fbxModel.pIndices = new unsigned int[fbxModel.numberOfIndices];

        unsigned int indexOffset = 0;
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];

            for (unsigned int j = 0; j < 3; j++)
            {
                fbxModel.pIndices[indexOffset + j] = face.mIndices[j];
            }

            indexOffset += 3;
        }
    }

    // Recursively process all child nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene, fbxModel);
    }

    delete[] modelArray;
}

