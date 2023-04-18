#pragma once

#include <ECS/Component.h>

#include "../cFBOManager/cFBO.h"

struct FBOComponent : Component {
public:

	// Constructor
	FBOComponent() :
		componentType("FBOComponent"),
		fboName(""),
		frameBuffer(nullptr),
		FBOTextureID(NULL),
		width(NULL),
		height(NULL),
		useFBO(true),
		doOnce(true)
	{};

	// Destructor
	~FBOComponent() {};

	// The FBO instance
	cFBO* frameBuffer;

	// name of the frame buffer
	std::string fboName;

	// texture ID
	unsigned int FBOTextureID;

	// dimensions
	float width, height;

	// If this instance of FBO is to be used
	bool useFBO;

	// throwaway
	bool doOnce;

private:
	// name of the component
	std::string componentType;
};