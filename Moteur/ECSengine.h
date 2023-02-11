#include <ECS/System.h>
#include <vector>

#include "TransformComponent.h"

#include "EntityManager.h"
#include "ShaderSystem.h"
#include "RenderSystem.h"
#include "Window.h"

class ECSengine {
public:
	ECSengine();
	~ECSengine();

	void Initialize();
	void Shutdown();

	void Update(float dt);
	void Render();

	void CreateWindow(const char* title, const int width, const int height, bool fullScreen);
	void CreateShaderProgramFromFiles(unsigned int& id, const char* vertShader, const char* fragShader);
	unsigned int CreateEntity();

	Window* GetWindow();

	Component* AddComponent(unsigned int entityId, std::string componentType)
	{
		return entityManager.AddComponent(entityId, componentType);
	}

	void RemoveComponent(unsigned int entityId, std::string componentType)
	{
		entityManager.RemoveComponent(entityId, componentType);
	}

	void AddSystem(System* newSystem);
	void RemoveSystem(System* removeSystem);

private:
	EntityManager entityManager;
	
	RenderSystem* renderSystem;
	ShaderSystem* shaderSystem;

	std::vector<System*> systems;
};