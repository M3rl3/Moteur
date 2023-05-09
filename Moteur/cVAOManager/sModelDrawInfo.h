#pragma once

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

struct BonesInfo
{
	/*id is index in finalBoneMatrices*/
	int id;

	/*offset matrix transforms vertex from model space to bone space*/
	glm::mat4 offset;
};

// The vertex structure 
// that's ON THE GPU (eventually) 
// So dictated from THE SHADER
struct vertLayout
{
	float x, y, z, w;
	float r, g, b, a;
	float nx, ny, nz, nw;
	float u0, v0, u1, v1;
	float tx, ty, tz, tw;
	float bx, by, bz, bw;
	float BoneID[4];
	float BoneWeight[4];
};

struct sModelDrawInfo
{
	sModelDrawInfo();

	std::string meshName;

	unsigned int VAO_ID;

	unsigned int VertexBufferID;
	unsigned int VertexBuffer_Start_Index;
	unsigned int numberOfVertices;

	unsigned int IndexBufferID;
	unsigned int IndexBuffer_Start_Index;
	unsigned int numberOfIndices;
	unsigned int numberOfTriangles;
	unsigned int numberOfBones;

	std::unordered_map<std::string, BonesInfo> m_BoneInfoMap;
	int m_BoneCounter = 0;

	// The "local" (i.e. "CPU side" temporary array)
	vertLayout* pVertices;	//  = 0;
	// The index buffer (CPU side)
	unsigned int* pIndices;

	// You could store the max and min values of the 
	//  vertices here (determined when you load them):
	float maxX, maxY, maxZ;
	float minX, minY, minZ;
	// These are the lengths of the bounding box that holds the model, 
	//	so extentX = maxX - minX, etc. 
	float extentX, extentY, extentZ;

	//  scale = 1.5/maxExtent --> 1.5 x 1.5 x 1.5
	//	scale = 5.0/maxExtent --> 5 x 5 x 5
	float maxExtent;

	void CalculateExtents(void);

	std::unordered_map<std::string, BonesInfo>& GetBoneInfoMap();
	int& GetBoneCount();
};