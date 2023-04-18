#pragma once

#include <ECS/Component.h>

#include <glm/vec4.hpp>

enum TextureFormat {
	BMP,
	PNG
};

// Struct holding texture information
struct TextureComponent : Component{
public:

	// Constructor
	TextureComponent() :
		componentType("TextureComponent"),
		textureFormat(BMP),
		rgbaColor(glm::vec4(0.f)),
		useRGBAColor(false),
		useTexture(false),
		isReflective(false),
		isTransparent(false)
	{
		// Iterate through all textures and set them to null
		for (unsigned int i = 0; i < 8; i++) {
			textureID[i] = -1;
		}
		for (unsigned int i = 0; i < 8; i++) {
			textures[i] = "";
		}
		for (unsigned int i = 0; i < 8; i++) {
			textureRatios[i] = 0.f;
		}
	};

	// Destructor
	~TextureComponent() {};

	// File format of the texture
	TextureFormat textureFormat;

	// ID number for the Texture
	unsigned int textureID[8];

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

	// Reflective surface
	bool isReflective;

	// Refractive surface
	bool isTransparent;

private:

	// name of the component
	std::string componentType;
};