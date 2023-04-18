#pragma once

#include "cFBO.h"

#include <string>
#include <map>

class cFBOManager {
public:

	cFBOManager();
	~cFBOManager();

	bool CreateFrameBuffer(std::string FBOname, int width, int height);

	bool FindFBOInfoByName(std::string FBOname, cFBO& fboInfo);

	cFBO* GetFBOInfoByName(std::string FBOname);

	void Shutdown();

private:
	std::map<std::string, cFBO*> m_frameBufferObjects;
};