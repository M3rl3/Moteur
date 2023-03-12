#include "cPNGTexture.h"

#include <stb_image/stb_image.h>

cPNGTexture::cPNGTexture(unsigned int& textID, const std::string& path)
	: textureID(0), filePath(path), localBuffer(nullptr), width(0), height(0), bpp(0)
{
	stbi_set_flip_vertically_on_load(1);
	
	// create a display list
	GLuint displayList = glGenLists(1);
	localBuffer = stbi_load(path.c_str(), &width, &height, &bpp, 4);

	// compile the display list
	glNewList(displayList, GL_COMPILE);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEndList();

	// execute the display list script
	glCallList(displayList);

	if (localBuffer) {
		stbi_image_free(localBuffer);
	}

	textID = textureID;
}

cPNGTexture::~cPNGTexture()
{
	GLuint deleteTextureList = glGenLists(1);
	glNewList(deleteTextureList, GL_COMPILE);
	glDeleteTextures(1, &textureID);
	glEndList();

	glCallList(deleteTextureList);
}

void cPNGTexture::Bind(unsigned int slot) const
{
	GLuint bindTextureList = glGenLists(1);
	glNewList(bindTextureList, GL_COMPILE);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glEndList();

	glCallList(bindTextureList);
}

void cPNGTexture::Unbind() const
{
	GLuint unbindList = glGenLists(1);
	glNewList(unbindList, GL_COMPILE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEndList();

	glCallList(unbindList);
}
