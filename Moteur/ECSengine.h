#include <ECS/System.h>
#include <vector>

#include "TransformComponent.h"
#include "MeshComponent.h"
#include "ShaderComponent.h"
#include "TextureComponent.h"

#include "EntityManager.h"
#include "ShaderSystem.h"
#include "RenderSystem.h"
#include "MeshSystem.h"

#include "Window.h"

class ECSengine {
public:
	ECSengine();
	~ECSengine();

	void Shutdown();

	void Update(float dt);
	void Render();

	unsigned int CreateEntity();

	template <class T>
	T* AddComponent(unsigned int entityId)
	{
		return entityManager->AddComponent<T>(entityId);
	}

	template <class T>
	T* RemoveComponent(unsigned int entityId)
	{
		return entityManager->RemoveComponent<T>(entityId);
	}

	EntityManager* GetEntityManager() {
		return entityManager;
	}

	void AddSystem(System* newSystem);
	void RemoveSystem(System* removeSystem);

private:
	EntityManager* entityManager;
	
	RenderSystem* renderSystem;
	ShaderSystem* shaderSystem;
	MeshSystem* meshSystem;

	std::vector<System*> systems;
};