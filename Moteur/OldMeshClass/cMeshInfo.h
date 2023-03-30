#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../BasicPhysics/Particle.h"
#include "../cVAOManager/cVAOManager.h"
#include "../cAnimationManager/Animation.h"

class cMeshInfo {

public:

	// Constructor
	cMeshInfo();

	// Destructor
	~cMeshInfo();

	// Name of the mesh
	std::string meshName;

	// Another name
	std::string friendlyName;

	// Transform and color
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 velocity;
	glm::vec3 target;
	glm::quat rotation;
	glm::vec4 colour;
	glm::vec4 RGBAColour;
	glm::vec3 facingDirection;

	int counter = 0;
	float radius;
	float rotationAngle;
	bool hasCompletedRotation;
	bool completed = false;
	bool enabled;

	//float scale;

	// Misc info

	bool isWireframe;
	bool isVisible;
	bool drawBBox;
	bool useRGBAColour;
	bool hasTexture;
	bool teleport;
	bool doNotLight;
	bool isTerrainMesh;
	bool isSkyBoxMesh;
	bool hasChildMeshes;

	// local copy of the number of triangles
	int nTriangles;

	// local copy of the number of indices
	int nIndices;

	// local copy of the number of vertices
	int nVertices;

	// All the textures applied to the current mesh
	std::string textures[8];

	// How all these texures blend with each other
	float textureRatios[8];

	// Physics particle
	Particle* particle;

	// Animations applied to it
	Animation animation;

	// local copy of the vertices
	std::vector <glm::vec3> vertices;

	// local copy of the vertices
	std::vector <glm::vec3> indices;

	// Min and max extents
	glm::vec3 min;
	glm::vec3 max;

	// set a location to translate mesh to
	void LockTarget(glm::vec3 target);

	// Add the velocity to the position with some delta time
	void TranslateOverTime(float dt);

	// Add the velocity to the position with some delta time
	void TranslateOverTime(glm::vec3 velocity, float dt);

	// Remove all forces being applied
	void KillAllForces();

	// Set the rotation based on euler angles
	void SetRotationFromEuler(glm::vec3 newEulerAngleXYZ);

	// Adjust euler angle and convert it to a quaternion
	void AdjustRoationAngleFromEuler(glm::vec3 EulerAngleXYZ_Adjust);

	// Set a uniform scale
	void SetUniformScale(float newScale);

	// Initialize all the texture ratios to a value
	void SetTextureRatiosAcrossTheBoard(float newTextureRatio);

	// Make a local copy of the vertices
	void CopyVertices(sModelDrawInfo model);

	// Make a local copy of the indices
	void CopyIndices(sModelDrawInfo model);

	// All child meshes go here
	std::vector <cMeshInfo*> vecChildMeshes;
};