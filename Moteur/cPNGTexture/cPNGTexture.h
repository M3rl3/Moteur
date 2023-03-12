#pragma once

#include "../OpenGL.h"

#include <string>

class cPNGTexture {
public:
	cPNGTexture(unsigned int& textureID, const std::string& path);
	~cPNGTexture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetTextureID() const { return textureID; }
	inline int GetWidth() const { return width; }
	inline int GetHeight() const { return height; }

private:
	unsigned int textureID;
	std::string filePath;
	unsigned char* localBuffer;
	int height, width, bpp;
};