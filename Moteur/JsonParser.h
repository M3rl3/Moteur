#ifndef _JSONPARSER_H_
#define _JSONPARSER_H_

#include <string>
#include <vector>
#include "glm\vec3.hpp";
#include "glm\vec4.hpp";

class CJsonParser
{
private:
	static CJsonParser* m_pInstance;

public:
	static CJsonParser* GetInstance();
private:
	void DestroyInstance();

public:
	struct sSampleData
	{
		std::string MODELPATH;
		std::string MODELNAME;
		glm::vec3 POSITION;
		glm::vec3 ROTATION;
		glm::vec3 SCALE;
		glm::vec4 COLOR;
	};

private:
	explicit CJsonParser();
	~CJsonParser();
	void Destroy();

public:
	void LoadSampleFunction(std::string path, std::string fileName, std::vector<sSampleData>& vec);

};

#endif //_JSONPARSER_H_