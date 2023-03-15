#include "cTextureManager.h"

#include <sstream>

void cTextureManager::SetBasePath(std::string basepath)
{
	this->m_basePath = basepath;
	return;
}

bool cTextureManager::Create2DTextureFromPNGFile(std::string textureFileName, bool bGenerateMIPMap) 
{
	std::string fileToLoadFullPath = this->m_basePath + "/" + textureFileName;

	cPNGTexture* pTempTexture = new cPNGTexture();
	if (!pTempTexture->Create2DTextureFromPNGFile(textureFileName, fileToLoadFullPath, /*textureUnit,*/ bGenerateMIPMap))
	{
		this->m_appendErrorString("Can't load ");
		this->m_appendErrorString(fileToLoadFullPath);
		this->m_appendErrorString("\n");
		return false;
	}

	this->m_map_PNGTexNameToTexture[textureFileName] = pTempTexture;

	return true;
}


bool cTextureManager::Create2DTextureFromBMPFile( std::string textureFileName, bool bGenerateMIPMap )
{
	std::string fileToLoadFullPath = this->m_basePath + "/" + textureFileName;


	CTextureFromBMP* pTempTexture = new CTextureFromBMP();
	if ( ! pTempTexture->CreateNewTextureFromBMPFile2( textureFileName, fileToLoadFullPath, /*textureUnit,*/ bGenerateMIPMap ) )
	{
		this->m_appendErrorString( "Can't load " );
		this->m_appendErrorString( fileToLoadFullPath );
		this->m_appendErrorString( "\n" );
		return false;
	}

	// Texture is loaded OK
	//this->m_nextTextureUnitOffset++;
	
	this->m_map_TexNameToTexture[ textureFileName ] = pTempTexture;

	return true;
}


void cTextureManager::m_appendErrorString( std::string nextErrorText )
{
	std::stringstream ss;
	ss << this->m_lastError << nextErrorText;
	this->m_lastError = ss.str();
	return;
}

GLuint cTextureManager::getTextureIDFromName( std::string textureFileName )
{
	std::map< std::string, CTextureFromBMP* >::iterator itTexture
		= this->m_map_TexNameToTexture.find( textureFileName );
	// Found it?
	if ( itTexture == this->m_map_TexNameToTexture.end() )
	{
		return 0;
	}
	// Reutrn texture number (from OpenGL genTexture)
	return itTexture->second->getTextureNumber();
}

GLuint cTextureManager::getPNGTextureIDFromName(std::string textureFileName)
{
	std::map< std::string, cPNGTexture* >::iterator itTexture
		= this->m_map_PNGTexNameToTexture.find(textureFileName);
	// Found it?
	if (itTexture == this->m_map_PNGTexNameToTexture.end())
	{
		return 0;
	}

	// Reutrn texture number (from OpenGL genTexture)
	return itTexture->second->GetTextureID();
}


void cTextureManager::m_appendErrorStringLine( std::string nextErrorTextLine )
{
	std::stringstream ss;
	ss << this->m_lastError << std::endl;
	ss << nextErrorTextLine << std::endl;
	this->m_lastError = ss.str();
	return;
}


// Picks a random texture from the textures loaded
std::string cTextureManager::PickRandomTexture(void)
{
	if ( this->m_map_TexNameToTexture.empty() )
	{
		// There are no textures loaded, yet.
		return "";
	}

	int textureIndexRand = rand() % (this->m_map_TexNameToTexture.size() + 1);
	if ( textureIndexRand >= this->m_map_TexNameToTexture.size() )
	{
		textureIndexRand = 0;
	}

	std::map< std::string, CTextureFromBMP* >::iterator itTex = this->m_map_TexNameToTexture.begin();
	for ( unsigned int count = 0; count != textureIndexRand; count++, itTex++ ) 
	{}

	return itTex->second->getTextureName();
}


bool cTextureManager::CreateCubeTextureFromBMPFiles( 
                                    std::string cubeMapName, 
		                            std::string posX_fileName, std::string negX_fileName, 
		                            std::string posY_fileName, std::string negY_fileName, 
									std::string posZ_fileName, std::string negZ_fileName, 
									bool bIsSeamless, std::string &errorString )
{
	std::string posX_fileName_FullPath = this->m_basePath + "/" + posX_fileName;
	std::string negX_fileName_FullPath = this->m_basePath + "/" + negX_fileName;
	std::string posY_fileName_FullPath = this->m_basePath + "/" + posY_fileName;
	std::string negY_fileName_FullPath = this->m_basePath + "/" + negY_fileName;
	std::string posZ_fileName_FullPath = this->m_basePath + "/" + posZ_fileName;
	std::string negZ_fileName_FullPath = this->m_basePath + "/" + negZ_fileName;

	GLenum errorEnum;
	std::string errorDetails;
	CTextureFromBMP* pTempTexture = new CTextureFromBMP();
	if ( ! pTempTexture->CreateNewCubeTextureFromBMPFiles( 
				cubeMapName, 
				posX_fileName_FullPath, negX_fileName_FullPath, 
	            posY_fileName_FullPath, negY_fileName_FullPath, 
	            posZ_fileName_FullPath, negZ_fileName_FullPath, 
	            bIsSeamless, errorEnum, errorString, errorDetails ) )
	{
		this->m_appendErrorString( "Can't load " );
		this->m_appendErrorString( cubeMapName );
		this->m_appendErrorString( " because:\n" );
		this->m_appendErrorString( errorString );
		this->m_appendErrorString( "\n" );
		this->m_appendErrorString( errorDetails );
		errorString += ("\n" + errorDetails + "\n");
		return false;
	}//if ( ! pTempTexture->CreateNewCubeTextureFromBMPFiles()
	
	this->m_map_TexNameToTexture[ cubeMapName ] = pTempTexture;

	return true;
}

bool cTextureManager::CreateCubeTextureFromPNGFiles(std::string cubeMapName,
	std::string posX_fileName, std::string negX_fileName,
	std::string posY_fileName, std::string negY_fileName,
	std::string posZ_fileName, std::string negZ_fileName,
	bool bIsSeamless, std::string& errorString)
{
	std::string posX_fileName_FullPath = this->m_basePath + "/" + posX_fileName;
	std::string negX_fileName_FullPath = this->m_basePath + "/" + negX_fileName;
	std::string posY_fileName_FullPath = this->m_basePath + "/" + posY_fileName;
	std::string negY_fileName_FullPath = this->m_basePath + "/" + negY_fileName;
	std::string posZ_fileName_FullPath = this->m_basePath + "/" + posZ_fileName;
	std::string negZ_fileName_FullPath = this->m_basePath + "/" + negZ_fileName;

	GLenum errorEnum;
	std::string errorDetails;
	cPNGTexture* pTempTexture = new cPNGTexture();
	if (!pTempTexture->CreateNewCubeTextureFromPNGFiles(
		cubeMapName,
		posX_fileName_FullPath, negX_fileName_FullPath,
		posY_fileName_FullPath, negY_fileName_FullPath,
		posZ_fileName_FullPath, negZ_fileName_FullPath,
		bIsSeamless, errorEnum, errorString, errorDetails))
	{
		this->m_appendErrorString("Can't load ");
		this->m_appendErrorString(cubeMapName);
		this->m_appendErrorString(" because:\n");
		this->m_appendErrorString(errorString);
		this->m_appendErrorString("\n");
		this->m_appendErrorString(errorDetails);
		errorString += ("\n" + errorDetails + "\n");
		return false;
	}//if ( ! pTempTexture->CreateNewCubeTextureFromBMPFiles())

	this->m_map_PNGTexNameToTexture[cubeMapName] = pTempTexture;

	return true;
}