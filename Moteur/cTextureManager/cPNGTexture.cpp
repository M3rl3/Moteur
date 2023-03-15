#include "cPNGTexture.h"

#include <stb_image/stb_image.h>

cPNGTexture::cPNGTexture() 
	: textureID(0), textureName(""), filePath(""), localBuffer(nullptr), width(0), height(0), bpp(0) 
{
}

// Constructor loads the texture and "returns" its ID
cPNGTexture::cPNGTexture(unsigned int& textID, const std::string& path)
	: textureID(0), filePath(path), localBuffer(nullptr), width(0), height(0), bpp(0)
{

	// flip the texture vertically
	// since openGL expects the texture pixels to 
	// start from the bottom left, and not from the top left
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

// Destructor
cPNGTexture::~cPNGTexture()
{
	GLuint deleteTextureList = glGenLists(1);
	glNewList(deleteTextureList, GL_COMPILE);
	glDeleteTextures(1, &textureID);
	glEndList();

	glCallList(deleteTextureList);
}

bool cPNGTexture::Create2DTextureFromPNGFile(std::string textureName, const std::string& path, bool generateMIPMap)
{

	// Clear old openGL errors
	int IHateYou = glGetError();

	// flip the texture vertically
	// since openGL expects the texture pixels to 
	// start from the bottom left, and not from the top left
	stbi_set_flip_vertically_on_load(1);

	// Load the texture from the PNG file into this buffer
	this->localBuffer = stbi_load(path.c_str(), &this->width, &this->height, &this->bpp, 4);

	// Initialize textureID
	this->textureID = 0;
	glGenTextures(1, &this->textureID);

	// If there were errors
	if ((glGetError() & GL_INVALID_VALUE) == GL_INVALID_VALUE)
	{
		return false;
	}

	this->filePath = path;
	this->textureName = textureName;

	glBindTexture(GL_TEXTURE_2D, this->textureID);

	// In case texture is oddly aligned, set the client alignment to 1 byte (default is 4)
	GLint GL_UNPACK_ALIGNMENT_old = 0;
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &GL_UNPACK_ALIGNMENT_old);

	// Set alignment to 1 byte
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 
				 0, 
				 GL_RGBA8, 
				 this->width, 
				 this->height, 
				 0, 
				 GL_RGBA, 
				 GL_UNSIGNED_BYTE, 
				 this->localBuffer);

	// If there were errors
	if (this->bWasThereAnOpenGLError()) { return false; }
	
	// Put the pixel store aligment back to what it was
	glPixelStorei(GL_UNPACK_ALIGNMENT, GL_UNPACK_ALIGNMENT_old);

	// Cleanup
	if (this->localBuffer) {
		stbi_image_free(this->localBuffer);
	}

	// Gen mipmaps
	if (generateMIPMap) {
		glGenerateMipmap(GL_TEXTURE_2D);		// OpenGL 4.0
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

bool cPNGTexture::CreateNewCubeTextureFromPNGFiles(std::string cubeMapName,
	std::string posX_fileName, std::string negX_fileName,
	std::string posY_fileName, std::string negY_fileName,
	std::string posZ_fileName, std::string negZ_fileName,
	bool bIsSeamless,
	GLenum& errorEnum, std::string& errorString, std::string& errorDetails) 
{
	// Clear old openGL errors
	int IHateYou = glGetError();

	// flip the texture vertically
	// since openGL expects the texture pixels to 
	// start from the bottom left, and not from the top left
	stbi_set_flip_vertically_on_load(1);

	// Load the texture from the PNG files into the cube map buffer
	this->localBuffer_cube[0] = stbi_load(posX_fileName.c_str(), &this->width, &this->height, &this->bpp, 4);
	this->localBuffer_cube[1] = stbi_load(negX_fileName.c_str(), &this->width, &this->height, &this->bpp, 4);
	this->localBuffer_cube[2] = stbi_load(posY_fileName.c_str(), &this->width, &this->height, &this->bpp, 4);
	this->localBuffer_cube[3] = stbi_load(negY_fileName.c_str(), &this->width, &this->height, &this->bpp, 4);
	this->localBuffer_cube[4] = stbi_load(posZ_fileName.c_str(), &this->width, &this->height, &this->bpp, 4);
	this->localBuffer_cube[5] = stbi_load(negZ_fileName.c_str(), &this->width, &this->height, &this->bpp, 4);

	// Initialize textureID
	this->textureID = 0;
	glGenTextures(1, &this->textureID);

	// If there were errors
	if ((glGetError() & GL_INVALID_VALUE) == GL_INVALID_VALUE)
	{
		return false;
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID);

	if (bIsSeamless)
	{
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);		// OpenGL 4.1, maybe
	}

	if (this->bWasThereAnOpenGLError(errorEnum, errorString, errorDetails)) { return false; }

	glTexStorage2D(GL_TEXTURE_CUBE_MAP,
				   10,					// Mipmap levels
				   GL_RGBA8,			// Internal format
				   this->width,			// width (pixels)
				   this->height);		// height (pixels)

	if (this->bWasThereAnOpenGLError(errorEnum, errorString, errorDetails)) { return false; }

	// Assuming all images have the same dimensions

	// Positive X image
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		0, 
		0, 0,
		this->width,		// width
		this->height,		// height	
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		this->localBuffer_cube[0]);
	
	// Negative X image
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		0, 
		0, 0,
		this->width,		// width
		this->height,		// height
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		this->localBuffer_cube[1]);
	
	// Positive Y image
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		0, 
		0, 0,
		this->width,		// width
		this->height,		// height
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		this->localBuffer_cube[2]);
	
	// Negative Y image
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		0, 
		0, 0,
		this->width,		// width
		this->height,		// height
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		this->localBuffer_cube[3]);
	
	// Positive Z image
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		0, 
		0, 0,
		this->width,		// width
		this->height,		// height
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		this->localBuffer_cube[4]);
	
	// Negative Z image
	glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		0, 
		0, 0,
		this->width,		// width
		this->height,		// height
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		this->localBuffer_cube[5]);

	this->textureName = cubeMapName;

	for (int i = 0; i < 6; i++) {
		// Cleanup
		if (this->localBuffer_cube[i]) {
			stbi_image_free(this->localBuffer_cube[i]);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return true;
}

// Bind and set the active texture
void cPNGTexture::Bind(unsigned int textureUnit) const
{
	GLuint bindTextureList = glGenLists(1);
	glNewList(bindTextureList, GL_COMPILE);
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glEndList();

	glCallList(bindTextureList);
}

// Unbind an already bound texture
void cPNGTexture::Unbind() const
{
	GLuint unbindList = glGenLists(1);
	glNewList(unbindList, GL_COMPILE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEndList();

	glCallList(unbindList);
}

bool cPNGTexture::bWasThereAnOpenGLError(void)
{
	GLenum errorEnum = GL_NO_ERROR;
	std::string errorString;
	std::string errorDetails;
	return this->bWasThereAnOpenGLError(errorEnum, errorString, errorDetails);
}

bool cPNGTexture::bWasThereAnOpenGLError(GLenum& errorEnum)
{
	std::string errorString;
	std::string errorDetails;
	return this->bWasThereAnOpenGLError(errorEnum, errorString, errorDetails);
}

bool cPNGTexture::bWasThereAnOpenGLError(GLenum& errorEnum, std::string& errorString)
{
	std::string errorDetails;
	return this->bWasThereAnOpenGLError(errorEnum, errorString, errorDetails);
}

// Reutn false if everything is OK
bool cPNGTexture::bWasThereAnOpenGLError(GLenum& errorEnum, std::string& errorString, std::string& errorDetails)
{
	errorEnum = glGetError();
	// from: https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGetError.xml
	switch (errorEnum)
	{
	case GL_NO_ERROR:
		errorString = "GL_NO_ERROR";
		errorDetails = "GL_NO_ERROR: No error has been recorded.";
		/******************/
		return false;
		/******************/
		break;
	case GL_INVALID_ENUM:
		errorString = "GL_INVALID_ENUM";
		errorDetails = "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
		break;
	case GL_INVALID_VALUE:
		errorString = "GL_INVALID_VALUE";
		errorDetails = "GL_INVALID_VALUE: A numeric argument is out of range.";
		break;
	case GL_INVALID_OPERATION:
		errorString = "GL_INVALID_OPERATION";
		errorDetails = "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
		errorDetails = "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
		break;
	case GL_OUT_OF_MEMORY:
		errorString = "GL_OUT_OF_MEMORY";
		errorDetails = "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
		break;
	case GL_STACK_UNDERFLOW:
		errorString = "GL_STACK_UNDERFLOW";
		errorDetails = "GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow.";
		break;
	case GL_STACK_OVERFLOW:
		errorString = "GL_STACK_OVERFLOW";
		errorDetails = "GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow.";
		break;
	default:
		errorString = "Unknown error";
		errorDetails = "Unknown error";
		break;
	};

	return true;	// There -WAS- an error
}