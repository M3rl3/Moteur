#pragma once

#include <ECS/Component.h>

#include <glm/vec4.hpp>

// Struct holding texture information
struct TextureComponent : Component{
public:

	// Constructor
	TextureComponent() :
		componentType("TextureComponent"),
		textureID(NULL),
		rgbaColor(glm::vec4(0.f)),
		useRGBAColor(false),
		useTexture(false)
	{
		// Iterate through all textures and set them to null
		for (int i = 0; i < 8; i++) {
			textures[i] = "";
		}
		for (int i = 0; i < 8; i++) {
			textureRatios[i] = 0.f;
		}
	};

	// Destructor
	~TextureComponent() {};

	// ID number for the Texture
	unsigned int textureID;

	// The array of all textures related to the current entity
	std::string textures[8];

	// How the textures blend with each other
	float textureRatios[8];

	// Color of the mesh model
	glm::vec4 rgbaColor;

	// Use the set rgbaColor
	bool useRGBAColor;

	// Use the set texture
	bool useTexture;

private:

	// name of the component
	std::string componentType;
};