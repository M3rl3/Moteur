#include "cModelFileLoader.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include <glm/glm.hpp>

Assimp::Importer assimpImporter;

void ProcessNode(aiNode* node, const aiScene* scene, sModelDrawInfo& fbxModel);

struct vertexLayout {

    float x, y, z;
    float nx, ny, nz;
    float r, g, b, a;
    float texture_u, texture_v;
};

struct triangleLayout {

    unsigned int triangleIndices[3];
};

cModelFileLoader::cModelFileLoader() {

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

    const aiScene* scene = assimpImporter.ReadFile(fileToLoadFullPath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene) {
        std::cout << "Could not load model file " << fileName << std::endl;
        return -1;
    }

    std::cout << "Loading " << fileName << std::endl;

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

void ProcessNode(aiNode* node, const aiScene* scene, sModelDrawInfo& fbxModel)
{
    vertexLayout* modelArray = NULL;

    // Process all meshes in this node
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {

        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        fbxModel.numberOfVertices = mesh->mNumVertices;
        fbxModel.numberOfTriangles = mesh->mNumFaces;
        fbxModel.numberOfIndices = mesh->mNumFaces * 3;

        modelArray = new vertexLayout[fbxModel.numberOfVertices];

        for (unsigned int count = 0; count != fbxModel.numberOfVertices; count++)
        {
            modelArray[count].x = mesh->mVertices[count].x;
            modelArray[count].y = mesh->mVertices[count].y;
            modelArray[count].z = mesh->mVertices[count].z;

            modelArray[count].nx = mesh->mNormals[count].x;
            modelArray[count].ny = mesh->mNormals[count].y;
            modelArray[count].nz = mesh->mNormals[count].z;

            if (mesh->HasVertexColors(0)) {

                modelArray[count].r = mesh->mColors[0][count].r;
                modelArray[count].g = mesh->mColors[0][count].g;
                modelArray[count].b = mesh->mColors[0][count].b;
                modelArray[count].a = mesh->mColors[0][count].a;
            }

            if (mesh->HasTextureCoords(0)) {

                modelArray[count].texture_u = mesh->mTextureCoords[0][count].x;
                modelArray[count].texture_v = mesh->mTextureCoords[0][count].y;
            }

            int breakpoint = 0;
        }

        // Copy the vertex data
        fbxModel.pVertices = new vertLayout[fbxModel.numberOfVertices];

        for (unsigned int index = 0; index != fbxModel.numberOfVertices; index++)
        {
            fbxModel.pVertices[index].x = modelArray[index].x;
            fbxModel.pVertices[index].y = modelArray[index].y;
            fbxModel.pVertices[index].z = modelArray[index].z;

            fbxModel.pVertices[index].r = modelArray[index].r;
            fbxModel.pVertices[index].g = modelArray[index].g;
            fbxModel.pVertices[index].b = modelArray[index].b;
            fbxModel.pVertices[index].a = modelArray[index].a;

            fbxModel.pVertices[index].nx = modelArray[index].nx;
            fbxModel.pVertices[index].ny = modelArray[index].ny;
            fbxModel.pVertices[index].nz = modelArray[index].nz;

            fbxModel.pVertices[index].u0 = modelArray[index].texture_u;
            fbxModel.pVertices[index].v0 = modelArray[index].texture_v;

            int breakpoint = 0;
        }

        fbxModel.pIndices = new unsigned int[fbxModel.numberOfIndices];

        // Copy the index data
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