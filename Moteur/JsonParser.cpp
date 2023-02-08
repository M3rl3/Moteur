#include "JsonParser.h"
#include <sstream>

#include "rapidjson\document.h"
#include "rapidjson\writer.h"
#include "rapidjson\stringbuffer.h"
#include "rapidjson\filereadstream.h"

using namespace std;
using namespace glm;
using namespace rapidjson;

CJsonParser* CJsonParser::m_pInstance = nullptr;
CJsonParser* CJsonParser::GetInstance()
{
	if (nullptr == m_pInstance)
		m_pInstance = new CJsonParser;
	return m_pInstance;
}

void CJsonParser::DestroyInstance()
{
	if (nullptr != m_pInstance)
	{
		m_pInstance->Destroy();
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

CJsonParser::CJsonParser()
{
}

CJsonParser::~CJsonParser()
{
}

void CJsonParser::Destroy()
{
}

void CJsonParser::LoadSampleFunction(string path, string fileName, vector<sSampleData>& vec)
{
	stringstream fullPath;
	fullPath << path << fileName;

	FILE* file;
	fopen_s(&file, fullPath.str().c_str(), "rb");
	const unsigned int BUFFER_SIZE = 1000;
	char readBuffer[BUFFER_SIZE];

	FileReadStream input(file, readBuffer, sizeof(readBuffer));
	Document doc;
	doc.ParseStream(input);

	const Value& dataArray = doc["array"];
	assert(dataArray.IsArray());
	for (unsigned int i = 0; i < dataArray.Size(); ++i)
	{
		sSampleData data;

		const Value& curData = dataArray[i];
		data.MESHID = curData["meshId"].GetString();

		data.POSITION.x = curData["posX"].GetFloat();
		data.POSITION.y = curData["posY"].GetFloat();
		data.POSITION.z = curData["posZ"].GetFloat();

		data.ROTATION.x = curData["rotX"].GetFloat();
		data.ROTATION.y = curData["rotY"].GetFloat();
		data.ROTATION.z = curData["rotZ"].GetFloat();

		data.SCALE.x = curData["scaleX"].GetFloat();
		data.SCALE.y = curData["scaleY"].GetFloat();
		data.SCALE.z = curData["scaleZ"].GetFloat();

		vec.push_back(data);
	}
}
