#include <ECS/System.h>
#include <vector>

#include "EntityManager.h"
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
	unsigned int CreateEntity();

	Window* GetWindow();

	template <class T>
	T* AddComponent(unsigned int entityId)
	{
		return entityManager.AddComponent<T>(entityId);
	}

	template <class T>
	T* RemoveComponent(unsigned int entityId)
	{
		return entityManager.RemoveComponent<T>(entityId);
	}

	void AddSystem(System* newSystem);
	void RemoveSystem(System* removeSystem);

private:
	EntityManager entityManager;

	RenderSystem* renderSystem;

	std::vector<System*> systems;
};