#include "cFBOManager.h"

#include <iostream>

cFBOManager::cFBOManager()
{
}

cFBOManager::~cFBOManager()
{
}

bool cFBOManager::CreateFrameBuffer(std::string FBOname, int width, int height)
{
	std::string errorString = "";
	cFBO* newFBO = new cFBO();

	if (!newFBO->init(width, height, errorString)) {
		std::cout << "FBO creation error: " << errorString << std::endl;
		return false;
	}
	
	// Store the fbo information into the map
	m_frameBufferObjects.emplace(FBOname, newFBO);

	// this->m_frameBufferObjects[FBOname] = newFBO;

	std::cout << "FBO " << FBOname << " created." << std::endl;

	return true;
}

bool cFBOManager::FindFBOInfoByName(std::string FBOname, cFBO& fboInfo)
{
	std::map<std::string, cFBO*>::iterator
		itFBOInfo = this->m_frameBufferObjects.find(FBOname);

	// Find it? 
	if (itFBOInfo == this->m_frameBufferObjects.end())
	{
		// Nope
		return false;
	}

	// Else we found FBO
	// ...so 'return' that information
	fboInfo = *itFBOInfo->second;

	return true;
}

cFBO* cFBOManager::GetFBOInfoByName(std::string FBOname)
{
	std::map<std::string, cFBO*>::iterator
		itFBOInfo = this->m_frameBufferObjects.find(FBOname);

	// Find it? 
	if (itFBOInfo == this->m_frameBufferObjects.end())
	{
		// Nope
		return nullptr;
	}

	return itFBOInfo->second;
}

void cFBOManager::Shutdown()
{
	for (auto& fbo : m_frameBufferObjects) {
		fbo.second->shutdown();

		delete fbo.second;
		fbo.second = nullptr;
	}

	m_frameBufferObjects.clear();
}
