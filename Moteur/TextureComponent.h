#pragma once

#include <ECS/Component.h>

#include <glm/vec4.hpp>

struct TextureComponent : Component{
public:

	TextureComponent() :
		componentType("TextureComponent"),
		textureID(NULL),
		rgbaColor(glm::vec4(0.f)),
		useRGBAColor(false)
	{};

	~TextureComponent() {};

	unsigned int textureID;
	std::string textures[8];
	float textureRatios[8];

	glm::vec4 rgbaColor;
	bool useRGBAColor;

private:

	std::string componentType;
};