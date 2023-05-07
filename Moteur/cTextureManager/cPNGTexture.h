#pragma once

#include "../OpenGL.h"

#include <string>

// Texture class for PNG format images
class cPNGTexture {
public:

	// Default constructor
	cPNGTexture();

	// Constructor that loads the texture and "returns" its ID
	cPNGTexture(unsigned int& textureID, const std::string& path);

	// Destructor
	~cPNGTexture();

	// Create a 2D texture from provided PNG file path
	bool Create2DTextureFromPNGFile(std::string textureName, const std::string& path, bool generateMIPMap = true);

	// Create a cubemap texture from provided PNG files
	bool CreateNewCubeTextureFromPNGFiles(std::string cubeMapName,
		std::string posX_fileName, std::string negX_fileName,
		std::string posY_fileName, std::string negY_fileName,
		std::string posZ_fileName, std::string negZ_fileName,
		bool bIsSeamless,
		GLenum& errorEnum, std::string& errorString, std::string& errorDetails);

	// Create a 2D texture from provided PNG file path
	bool Create2DTextureFromLocalBuffer(unsigned char* localBuffer, std::string textureName, const std::string& path, bool generateMIPMap = true);

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

	// Name assigned to the texture
	std::string textureName;

	// Local CPU side copy of the PNG texture
	unsigned char* localBuffer;

	// Local CPU side copy of the PNG texture cube
	unsigned char* localBuffer_cube[6];

	// Image metadata
	int height, width, bpp;

	// Error check
	bool bWasThereAnOpenGLError(void);
	bool bWasThereAnOpenGLError(GLenum& errorEnum);
	bool bWasThereAnOpenGLError(GLenum& errorEnum, std::string& errorString);
	bool bWasThereAnOpenGLError(GLenum& errorEnum, std::string& errorString, std::string& errorDetails);

};