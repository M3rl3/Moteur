#pragma once

#include "../OpenGL.h"

#include <string>

// Texture class for PNG format images
class cPNGTexture {
public:
	// Constructor loads the texture and "returns" its ID
	cPNGTexture(unsigned int& textureID, const std::string& path);

	// Destructor
	~cPNGTexture();

	// Bind and set the active texture
	void Bind(unsigned int textureUnit = 0) const;

	// Unbind an already bound texture
	void Unbind() const;

	// Returns the textureID for the current instance
	inline int GetTextureID() const { return textureID; }

	// Returns the width of the current PNG image in pixels
	inline int GetWidth() const { return width; }

	// Returns the height of the current PNG image in pixels
	inline int GetHeight() const { return height; }

private:
	// Identifier for the texture
	unsigned int textureID;

	// Path to the PNG image 
	std::string filePath;

	// Local CPU side copy of the PNG texture
	unsigned char* localBuffer;

	// Image metadata
	int height, width, bpp;
};