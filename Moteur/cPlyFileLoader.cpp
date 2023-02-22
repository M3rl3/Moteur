#include "cPlyFileLoader.h"

cPlyFileLoader::cPlyFileLoader() {

}

cPlyFileLoader::~cPlyFileLoader() {

}

sModelDrawInfo* cPlyFileLoader::GetPlyModelByID(unsigned int id) {

    return plyModels[id];
}

int cPlyFileLoader::LoadModel(std::string fileName, sModelDrawInfo& plyModel) {

    sModelDrawInfo model;

    vertexLayout* modelArray = NULL;
    triangleLayout* triangleArray = NULL;

    std::ifstream plyFile(fileName);
    if (!plyFile.is_open()) {
        std::cout << "Could not load file." << std::endl;
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

    plyFile >> model.numberOfVertices;

    // Scan for the word "face"
    while (plyFile >> input1)
    {
        if (input1 == "face")
        {
            break;
        }
    }

    plyFile >> model.numberOfTriangles;

    // Scan for the word "end_header"
    while (plyFile >> input1)
    {
        if (input1 == "end_header")
        {
            break;
        }
    }

    //Vertex Layout
    modelArray = new vertexLayout[model.numberOfVertices];

    std::cout << "Loading " << fileName << std::endl;
    for (unsigned int count = 0; count != model.numberOfVertices; count++)
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
        int breakpoint = 1;
    }

    triangleArray = new triangleLayout[model.numberOfTriangles];

    // Triangle Layout
    for (unsigned int count = 0; count != model.numberOfTriangles; count++) {
        unsigned int temp = 0;
        plyFile >> temp;

        plyFile >> triangleArray[count].triangleIndices[0];
        plyFile >> triangleArray[count].triangleIndices[1];
        plyFile >> triangleArray[count].triangleIndices[2];
        int breakpoint = 1;
    }
    plyFile.close();

    model.pVertices = new vertLayout[model.numberOfVertices];

    for (unsigned int index = 0; index != model.numberOfVertices; index++)
    {
        model.pVertices[index].x = modelArray[index].x;
        model.pVertices[index].y = modelArray[index].y;
        model.pVertices[index].z = modelArray[index].z;

        model.pVertices[index].r = modelArray[index].r;
        model.pVertices[index].g = modelArray[index].g;
        model.pVertices[index].b = modelArray[index].b;

        model.pVertices[index].nx = modelArray[index].nx;
        model.pVertices[index].ny = modelArray[index].ny;
        model.pVertices[index].nz = modelArray[index].nz;

        model.pVertices[index].texture_u = modelArray[index].texture_u;
        model.pVertices[index].texture_v = modelArray[index].texture_v;
    }

    model.numberOfIndices = model.numberOfTriangles * 3;
    model.pIndices = new unsigned int[model.numberOfIndices];

    unsigned int indexIndices = 0;

    for (unsigned int triangleIndex = 0; triangleIndex != model.numberOfTriangles; triangleIndex++)
    {
        model.pIndices[indexIndices + 0] = triangleArray[triangleIndex].triangleIndices[0];
        model.pIndices[indexIndices + 1] = triangleArray[triangleIndex].triangleIndices[1];
        model.pIndices[indexIndices + 2] = triangleArray[triangleIndex].triangleIndices[2];

        indexIndices += 3;
    }

    if (&model != nullptr) {

        plyModels.push_back(&model);
    }

    delete[] modelArray;
    delete[] triangleArray;

    plyModel = model;

    return plyModels.size() - 1;
}