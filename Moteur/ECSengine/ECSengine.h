#include <ECS/System.h>
#include <vector>

#include "../Components/TransformComponent.h"
#include "../Components/MeshComponent.h"
#include "../Components/ShaderComponent.h"
#include "../Components/TextureComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/VelocityComponent.h"
#include "../Components/BoundingBoxComponent.h"
#include "../Components/LitComponent.h"

#include "../EntityManager/EntityManager.h"
#include "../Systems/ShaderSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/MeshSystem.h"
#include "../Systems/MotionSystem.h"
#include "../Systems/LightSystem.h"

#include "../Window/Window.h"

class ECSengine {
public:
	// Constructor
	ECSengine();
	// Destructor
	~ECSengine();

	// Calls update on all systems currently present every tick
	void Update(float dt);

	// Callback for a user defined update method
	void UpdateCallback(void (*Callback)(float dt));

	// Creates a new entity and returns its ID
	unsigned int CreateEntity();

	// Calls shutdown on all added systems
	void Shutdown();

	// Add a component that inherits from class Component
	template <class T>
	T* AddComponent(unsigned int entityId)
	{
		return entityManager->AddComponent<T>(entityId);
	}

	// Remove a component
	template <class T>
	T* RemoveComponent(unsigned int entityId)
	{
		return entityManager->RemoveComponent<T>(entityId);
	}

	// Returns the entity manager
	EntityManager* GetEntityManager() {
		return entityManager;
	}

	// Add a system to array of systems
	void AddSystem(System* newSystem);

	// Remove a system from the array of systems
	void RemoveSystem(System* removeSystem);

private:
	// A manager for all entities that has an array them within it
	EntityManager* entityManager;

	// Array for all added systems
	std::vector<System*> systems;

	// User defined update method
	void (*UpdateCallBack)(float dt);
};