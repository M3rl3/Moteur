#include "Scene.h"

#include "Components/AIComponent.h"
#include "Components/TransformComponent.h"
#include "Components/MeshComponent.h"
#include "Components/ShaderComponent.h"
#include "Components/TextureComponent.h"
#include "Components/VelocityComponent.h"
#include "Components/BoundingBoxComponent.h"
#include "Components/LitComponent.h"
#include "Components/SoundComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/CharacterControllerComponent.h"
#include "Components/FBOComponent.h"
#include "Components/PlayerComponent.h"

#include "../PhysicsEngine/interfaces/CylinderShape.h"
#include "../PhysicsEngine/interfaces/CapsuleShape.h"
#include "../PhysicsEngine/interfaces/PlaneShape.h"
#include "../PhysicsEngine/interfaces/RigidBodyDesc.h"

#include <sstream>

// Globals
// 
// GameMode
GameMode gameMode;

// Player attributes
TransformComponent* transformComponent;
VelocityComponent* velocityComponent;
RigidBodyComponent* rigidBodyComponent;
CharacterControllerComponent* characterControllerComponent;

// Systems
RenderSystem* renderSys;

// Delta time
float elapsedTime = 0.f;

// Frame rate & Frame time
unsigned int frameCount = 0;
unsigned int frameRate = 0;
unsigned int frameTime = 0;

// Constructor
Scene::Scene()
{
    // Init Engine
	engine = new ECSengine();

    // Init Systems
	renderSystem = new RenderSystem();
	physicsSystem = new PhysicsSystem();
	motionSystem = new MotionSystem();
	shaderSystem = new ShaderSystem();
	lightSystem = new LightSystem();
	soundSystem = new SoundSystem();
    aiSystem = new AISystem();

    renderSys = renderSystem;

    // Set Game Mode
	gameMode = GameMode::CAMERA;

    // Khronos
    timer = new Timer();

    // Enemy Health
    enemyHealth = 500.f;
}

// Destructor
Scene::~Scene()
{

}

// Initialize everything
void Scene::Initialize() 
{
	renderSystem->Initialize("Man", 1704, 960, false);

	renderSystem->SetCameraPosition(glm::vec3(-30.f, 5.f, -50.f));
	renderSystem->SetCameraTarget(glm::vec3(0.f, 0.f, 1.f));

	// Shaders loaded here
	const char* v_Shader = "../assets/shaders/vertexShader.glsl";
	const char* g_Shader = "../assets/shaders/geometryShader_passthrough.glsl";
	const char* f_Shader = "../assets/shaders/fragmentShader.glsl";

    // Compile shader program
	shaderSystem->CreateShaderProgramFromFiles(shaderID, v_Shader, g_Shader, f_Shader);

	// Physics
	physicsSystem->SetGravity(glm::vec3(0.f, -9.8f, 0.f));

	// Lighting
	// 
	// Ambient light
	float ambientLight = 0.001f;
	lightSystem->SetAmbientLightAmount(ambientLight);

	// Add all the systems
	engine->AddSystem(renderSystem);
	engine->AddSystem(physicsSystem);
	engine->AddSystem(lightSystem);
	engine->AddSystem(shaderSystem);
	engine->AddSystem(motionSystem);
    engine->AddSystem(aiSystem);
	engine->AddSystem(soundSystem);
}

// Render the Scene
void Scene::Render() 
{
    // Entities
    CreateFlatPlane();

    {   // Entity "player"

        unsigned int entityID = engine->CreateEntity();

        transformComponent = engine->AddComponent<TransformComponent>(entityID);
        transformComponent->position = glm::vec3(95.f, 2.4f, 6.f);
        transformComponent->scale = glm::vec3(0.01f);
        transformComponent->rotation = glm::quat(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
        transformComponent->useModel = true;

        ShaderComponent* shaderComponent = engine->AddComponent<ShaderComponent>(entityID);
        shaderComponent->shaderID = shaderID;

        MeshComponent* meshComponent = engine->AddComponent<MeshComponent>(entityID);
        meshComponent->meshName = "merle";
        meshComponent->modelFormat = ModelFormat::FBX;
        meshComponent->isWireframe = false;
        meshComponent->useBones = false;
        meshComponent->useModelInfo = false;

        Animation* tpose;
        renderSystem->LoadMesh("../assets/meshes/castle_guard.fbx", "krupesh", meshComponent->model, shaderID);
        tpose = new Animation("../assets/meshes/castle_guard.fbx", &meshComponent->model);
        meshComponent->animator = new Animator(tpose);

        TextureComponent* textureComponent = engine->AddComponent<TextureComponent>(entityID);
        textureComponent->useTexture = true;
        textureComponent->textureFormat = TextureFormat::PNG;
        textureComponent->textures[0] = "noman.png";
        textureComponent->textureRatios[0] = 1.f;
        textureComponent->useRGBAColor = false;
        textureComponent->isTransparent = true;
        textureComponent->rgbaColor = glm::vec4(1.f, 1.f, 1.f, 1.f);

        LitComponent* litComponent = engine->AddComponent<LitComponent>(entityID);
        litComponent->isLit = true;

        SoundComponent* soundComponent = engine->AddComponent<SoundComponent>(entityID);
        soundComponent->isPlaying = true;
        soundComponent->maxDistance = 1.f;
        soundComponent->soundVolume = 5.f;
        soundComponent->soundName = "my_dark_disquiet.mp3";

        characterControllerComponent = engine->AddComponent<CharacterControllerComponent>(entityID);
        characterControllerComponent->convexShape = new physics::CapsuleShape(0.5f, 1.f);
        characterControllerComponent->stepHeight = 0.25f;
        characterControllerComponent->canJump = true;
        characterControllerComponent->isControllable = true;

        velocityComponent = engine->AddComponent<VelocityComponent>(entityID);
        velocityComponent->velocity = glm::vec3(0.f, 0.f, 0.f);
        velocityComponent->useVelocity = false;

        PlayerComponent* playerComponent = engine->AddComponent<PlayerComponent>(entityID);
        playerComponent->isPlayer = true;
        playerComponent->health = 1000.f;
    }

    {   // Entity "creepyMonster"

        unsigned int entityID = engine->CreateEntity();

        TransformComponent* transformComponent = engine->AddComponent<TransformComponent>(entityID);
        transformComponent->position = glm::vec3(100.f, 2.4f, 65.f);
        transformComponent->scale = glm::vec3(1.f);
        transformComponent->rotation = glm::quat(glm::vec3(0.f, 0.f, 0.f));

        ShaderComponent* shaderComponent = engine->AddComponent<ShaderComponent>(entityID);
        shaderComponent->shaderID = shaderID;

        MeshComponent* meshComponent = engine->AddComponent<MeshComponent>(entityID);
        meshComponent->meshName = "creepyMonster";
        meshComponent->modelFormat = ModelFormat::PLY;
        meshComponent->health = enemyHealth;
        meshComponent->isWireframe = false;

        TextureComponent* textureComponent = engine->AddComponent<TextureComponent>(entityID);
        textureComponent->useTexture = true;
        textureComponent->textureFormat = TextureFormat::BMP;
        textureComponent->textures[0] = "full_low_body__BaseColor.bmp";
        textureComponent->textureRatios[0] = 1.f;
        textureComponent->useRGBAColor = false;
        textureComponent->rgbaColor = glm::vec4(1.f, 1.f, 1.f, 1.f);

        RigidBodyComponent* rigidBodyComponent = engine->AddComponent<RigidBodyComponent>(entityID);
        rigidBodyComponent->bodyShape = new physics::CapsuleShape(1.f, 1.f);
        rigidBodyComponent->rigidBodyDesc.isStatic = false;
        rigidBodyComponent->rigidBodyDesc.useInertia = false;
        rigidBodyComponent->rigidBodyDesc.mass = 1.f;
        rigidBodyComponent->usePhysics = false;

        LitComponent* litComponent = engine->AddComponent<LitComponent>(entityID);
        litComponent->isLit = true;

        VelocityComponent* velocityComponent = engine->AddComponent<VelocityComponent>(entityID);
        velocityComponent->velocity = glm::vec3(0.f, 0.f, 0.f);
        velocityComponent->useVelocity = true;

        SoundComponent* soundComponent = engine->AddComponent<SoundComponent>(entityID);
        soundComponent->isPlaying = true;
        soundComponent->maxDistance = 0.25f;
        soundComponent->soundVolume = 1.f;
        soundComponent->soundName = "chicken.wav";

        AIComponent* aiComponent = engine->AddComponent<AIComponent>(entityID);
        aiComponent->aiBehaviour = BehaviourType::IDLE;
        aiComponent->radius = 10.f;
    }

    {   // Entity "female_warrior"

        unsigned int entityID = engine->CreateEntity();

        TransformComponent* transformComponent = engine->AddComponent<TransformComponent>(entityID);
        transformComponent->position = glm::vec3(100.f, 2.4f, 55.f);
        transformComponent->scale = glm::vec3(0.2f);
        transformComponent->rotation = glm::quat(glm::vec3(0.f, glm::radians(180.f), 0.f));

        ShaderComponent* shaderComponent = engine->AddComponent<ShaderComponent>(entityID);
        shaderComponent->shaderID = shaderID;

        MeshComponent* meshComponent = engine->AddComponent<MeshComponent>(entityID);
        meshComponent->meshName = "female_warrior";
        meshComponent->modelFormat = ModelFormat::PLY;
        meshComponent->health = enemyHealth;
        meshComponent->isWireframe = false;

        TextureComponent* textureComponent = engine->AddComponent<TextureComponent>(entityID);
        textureComponent->useRGBAColor = true;
        textureComponent->rgbaColor = glm::vec4(0.21f, 0.21f, 0.21f, 1.f);

        RigidBodyComponent* rigidBodyComponent = engine->AddComponent<RigidBodyComponent>(entityID);
        rigidBodyComponent->bodyShape = new physics::CylinderShape(glm::vec3(1.f));
        rigidBodyComponent->rigidBodyDesc.isStatic = false;
        rigidBodyComponent->rigidBodyDesc.useInertia = false;
        rigidBodyComponent->rigidBodyDesc.mass = 1.f;
        rigidBodyComponent->usePhysics = false;

        LitComponent* litComponent = engine->AddComponent<LitComponent>(entityID);
        litComponent->isLit = true;

        VelocityComponent* velocityComponent = engine->AddComponent<VelocityComponent>(entityID);
        velocityComponent->velocity = glm::vec3(0.f, 0.f, 0.f);
        velocityComponent->useVelocity = true;

        AIComponent* aiComponent = engine->AddComponent<AIComponent>(entityID);
        aiComponent->aiBehaviour = BehaviourType::IDLE;
        aiComponent->radius = 20.f;
    }
    
    {   // Entity "minotaur"

        unsigned int entityID = engine->CreateEntity();

        TransformComponent* transformComponent = engine->AddComponent<TransformComponent>(entityID);
        transformComponent->position = glm::vec3(105.f, 2.4f, 60.f);
        transformComponent->scale = glm::vec3(0.025);
        transformComponent->rotation = glm::quat(glm::vec3(0.f, glm::radians(90.f), 0.f));

        ShaderComponent* shaderComponent = engine->AddComponent<ShaderComponent>(entityID);
        shaderComponent->shaderID = shaderID;

        MeshComponent* meshComponent = engine->AddComponent<MeshComponent>(entityID);
        meshComponent->meshName = "minotaur";
        meshComponent->modelFormat = ModelFormat::PLY;
        meshComponent->health = enemyHealth;
        meshComponent->isWireframe = false;

        TextureComponent* textureComponent = engine->AddComponent<TextureComponent>(entityID);
        textureComponent->useRGBAColor = true;
        textureComponent->rgbaColor = glm::vec4(0.2f, 0.f, 0.f, 1.f);

        RigidBodyComponent* rigidBodyComponent = engine->AddComponent<RigidBodyComponent>(entityID);
        rigidBodyComponent->bodyShape = new physics::CapsuleShape(0.5f, 1.f);
        rigidBodyComponent->rigidBodyDesc.isStatic = false;
        rigidBodyComponent->rigidBodyDesc.useInertia = false;
        rigidBodyComponent->rigidBodyDesc.mass = 1.f;
        rigidBodyComponent->usePhysics = false;

        LitComponent* litComponent = engine->AddComponent<LitComponent>(entityID);
        litComponent->isLit = true;

        VelocityComponent* velocityComponent = engine->AddComponent<VelocityComponent>(entityID);
        velocityComponent->velocity = glm::vec3(0.f, 0.f, 0.f);
        velocityComponent->useVelocity = true;

        AIComponent* aiComponent = engine->AddComponent<AIComponent>(entityID);
        aiComponent->aiBehaviour = BehaviourType::IDLE;
        aiComponent->radius = 10.f;
    }

    {   // Entity "plane"

        unsigned int entityID = engine->CreateEntity();

        TransformComponent* transformComponent = engine->AddComponent<TransformComponent>(entityID);
        transformComponent->position = glm::vec3(0.f);
        transformComponent->scale = glm::vec3(0.1f);
        transformComponent->rotation = glm::quat(glm::vec3(0.f));

        ShaderComponent* shaderComponent = engine->AddComponent<ShaderComponent>(entityID);
        shaderComponent->shaderID = shaderID;

        MeshComponent* meshComponent = engine->AddComponent<MeshComponent>(entityID);
        meshComponent->meshName = "plane";
        meshComponent->modelFormat = ModelFormat::PLY;
        meshComponent->isWireframe = false;

        TextureComponent* textureComponent = engine->AddComponent<TextureComponent>(entityID);
        textureComponent->useRGBAColor = false;
        textureComponent->rgbaColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.f);
        textureComponent->useTexture = true;
        textureComponent->textureFormat = TextureFormat::BMP;
        textureComponent->textures[0] = "seamless-green-grass-pattern.bmp";
        textureComponent->textureRatios[0] = 0.1f;

        RigidBodyComponent* rigidBodyComponent = engine->AddComponent<RigidBodyComponent>(entityID);
        rigidBodyComponent->bodyShape = new physics::PlaneShape(1.0f, glm::vec3(0.f, 1.f, 0.f));
        rigidBodyComponent->rigidBodyDesc.isStatic = true;
        rigidBodyComponent->usePhysics = true;

        LitComponent* litComponent = engine->AddComponent<LitComponent>(entityID);
        litComponent->isLit = true;
    }

    {   // Entity "campfire"

        unsigned int entityID = engine->CreateEntity();

        TransformComponent* transformComponent = engine->AddComponent<TransformComponent>(entityID);
        transformComponent->position = glm::vec3(100.f, 1.9f, 60.f);
        transformComponent->scale = glm::vec3(0.04f);
        transformComponent->rotation = glm::quat(glm::vec3(0.f, 0.f, 0.f));

        ShaderComponent* shaderComponent = engine->AddComponent<ShaderComponent>(entityID);
        shaderComponent->shaderID = shaderID;

        MeshComponent* meshComponent = engine->AddComponent<MeshComponent>(entityID);
        meshComponent->meshName = "campfire";
        meshComponent->modelFormat = ModelFormat::PLY;

        RigidBodyComponent* rigidBodyComponent = engine->AddComponent<RigidBodyComponent>(entityID);
        rigidBodyComponent->bodyShape = new physics::CylinderShape(glm::vec3(1.f));
        rigidBodyComponent->rigidBodyDesc.isStatic = true;
        rigidBodyComponent->rigidBodyDesc.useInertia = false;
        rigidBodyComponent->usePhysics = true;

        TextureComponent* textureComponent = engine->AddComponent<TextureComponent>(entityID);
        textureComponent->textureFormat = TextureFormat::PNG;
        textureComponent->useTexture = true;
        textureComponent->textures[0] = "Campfire_MAT_BaseColor_00.png";
        textureComponent->textureRatios[0] = 0.1f;
        textureComponent->textures[1] = "Campfire_MAT_BaseColor_01.png";
        textureComponent->textureRatios[1] = 0.5f;
        textureComponent->textures[2] = "Campfire_MAT_Metallic.png";
        textureComponent->textureRatios[2] = 0.6f;
        textureComponent->textures[3] = "Campfire_MAT_Roughness.png";
        textureComponent->textureRatios[3] = 1.f;
        textureComponent->textures[4] = "Campfire_MAT_AO.png";
        textureComponent->textureRatios[4] = 0.01f;

        LitComponent* litComponent = engine->AddComponent<LitComponent>(entityID);
        litComponent->isLit = true;
    }
    
    {   // Entity "tree"

        unsigned int entityID = engine->CreateEntity();

        TransformComponent* transformComponent = engine->AddComponent<TransformComponent>(entityID);
        transformComponent->position = glm::vec3(120.f, 1.9f, 60.f);
        transformComponent->scale = glm::vec3(0.02f);
        transformComponent->rotation = glm::quat(glm::vec3(0.f, 0.f, 0.f));

        ShaderComponent* shaderComponent = engine->AddComponent<ShaderComponent>(entityID);
        shaderComponent->shaderID = shaderID;

        MeshComponent* meshComponent = engine->AddComponent<MeshComponent>(entityID);
        meshComponent->meshName = "tree";
        meshComponent->modelFormat = ModelFormat::PLY;

        RigidBodyComponent* rigidBodyComponent = engine->AddComponent<RigidBodyComponent>(entityID);
        rigidBodyComponent->bodyShape = new physics::CylinderShape(glm::vec3(1.f));
        rigidBodyComponent->rigidBodyDesc.isStatic = true;
        rigidBodyComponent->rigidBodyDesc.useInertia = false;
        rigidBodyComponent->usePhysics = true;

        TextureComponent* textureComponent = engine->AddComponent<TextureComponent>(entityID);
        textureComponent->textureFormat = TextureFormat::PNG;
        textureComponent->useTexture = true;
        textureComponent->textures[0] = "tree001.png";
        textureComponent->textureRatios[0] = 0.45f;
        textureComponent->textures[1] = "tree001_SubsurfaceColor.png";
        textureComponent->textureRatios[1] = 0.5f;
        textureComponent->textures[2] = "tree001_Gloss.png";
        textureComponent->textureRatios[2] = 0.01f;
        textureComponent->textures[3] = "tree001_Opacity.png";
        textureComponent->textureRatios[3] = 1.f;

        LitComponent* litComponent = engine->AddComponent<LitComponent>(entityID);
        litComponent->isLit = true;
    }
    
    {   // Entity "skybox"

        unsigned int entityID = engine->CreateEntity();

        TransformComponent* transformComponent = engine->AddComponent<TransformComponent>(entityID);

        ShaderComponent* shaderComponent = engine->AddComponent<ShaderComponent>(entityID);
        shaderComponent->shaderID = shaderID;

        MeshComponent* meshComponent = engine->AddComponent<MeshComponent>(entityID);
        meshComponent->meshName = "skybox";
        meshComponent->modelFormat = ModelFormat::PLY;
        meshComponent->isWireframe = false;
        meshComponent->isSkyBox = true;

        TextureComponent* textureComponent = engine->AddComponent<TextureComponent>(entityID);
        textureComponent->textureFormat = TextureFormat::PNG;
        textureComponent->textures[0] = "orangeSky";
    }
    
    // Set update callback
    engine->UpdateCallback(&UpdateCallback);

    // Load assets
    LoadAssets();
}

void Scene::Update(float dt) 
{
    // Update current time
    timer->Update();

    // check if update is available
    if (timer->IsUpdateAvailable()) {

        // Engine update
        engine->Update(dt);

        // Increment frame counter
        frameCount++;
    }

    // Get elapsed time 
    elapsedTime += timer->GetElapsedTime();

    // Update window title
    if (elapsedTime >= 1.f / 30.f)
    {
        // Window title
        std::stringstream ssTitle;

        ssTitle.str("");

        // Gets the GPU that's in use
        const GLubyte* vendor = glGetString(GL_VENDOR);
        const GLubyte* renderer = glGetString(GL_RENDERER);

        // Calculate Frame rate & Frame time
        frameRate = (1.f / elapsedTime) * frameCount;
        frameTime = (elapsedTime / frameCount) * 1000;

        ssTitle << "Camera: " << "(" <<
            renderSystem->GetCamera()->position.x << ", " <<
            renderSystem->GetCamera()->position.y << ", " <<
            renderSystem->GetCamera()->position.z << ") " <<
            " FPS: " << frameRate <<
            " ms: " << frameTime <<
            "   Renderer: " << renderer;

        // Set window title
        glfwSetWindowTitle(renderSystem->GetWindow()->theWindow, ssTitle.str().c_str());

        // Reset counters
        frameCount = 0;
        elapsedTime = 0.f;
    }
}

void Scene::UpdateCallback(float dt) 
{
    const float MOVE_SPEED = 1.f;

    if (renderSys->IsKeyPressed(GLFW_KEY_C)) {
        gameMode = CAMERA;
    }
    if (renderSys->IsKeyPressed(GLFW_KEY_F)) {
        gameMode = PLAYER;
    }

    switch (gameMode)
    {
    case CAMERA:

        if (renderSys->IsKeyHeldDown(GLFW_KEY_W)) {
            renderSys->GetCamera()->MoveForward();
        }
        if (renderSys->IsKeyHeldDown(GLFW_KEY_S)) {
            renderSys->GetCamera()->MoveBackward();
        }
        if (renderSys->IsKeyHeldDown(GLFW_KEY_D)) {
            renderSys->GetCamera()->StrafeRight();
        }
        if (renderSys->IsKeyHeldDown(GLFW_KEY_A)) {
            renderSys->GetCamera()->StrafeLeft();
        }
        if (renderSys->IsKeyHeldDown(GLFW_KEY_Q)) {
            renderSys->GetCamera()->MoveUp();
        }
        if (renderSys->IsKeyHeldDown(GLFW_KEY_E)) {
            renderSys->GetCamera()->MoveDown();
        }
        break;

    case PLAYER:

        if (renderSys->GetMouseStatus()) {

            glm::vec3 direction = renderSys->GetCamera()->target;
            glm::vec3 up = renderSys->GetCamera()->upVector;

            direction.y = 0.f;

            glm::quat rotation =
                glm::quat(glm::lookAt(transformComponent->position, velocityComponent->velocity, up))
              * glm::quat(glm::vec3(glm::radians(180.f), glm::radians(180.f), glm::radians(180.f)));

            float rotationAngle = 5.f * dt;
            glm::quat rotation_delta = glm::angleAxis(rotationAngle, up);
            
            rotation = rotation_delta * rotation;

            transformComponent->rotation = rotation;

            if (renderSys->IsKeyHeldDown(GLFW_KEY_W)) {
                velocityComponent->velocity += MOVE_SPEED * direction;
            }
            if (renderSys->IsKeyReleased(GLFW_KEY_W)) {
                velocityComponent->velocity = glm::vec3(0.f);
            }
            if (renderSys->IsKeyHeldDown(GLFW_KEY_S)) {
                velocityComponent->velocity += -MOVE_SPEED * direction;
            }
            if (renderSys->IsKeyReleased(GLFW_KEY_S)) {
                velocityComponent->velocity = glm::vec3(0.f);
            }
            if (renderSys->IsKeyHeldDown(GLFW_KEY_D)) {
                glm::vec3 strafeDirection = glm::cross(direction, up);
                velocityComponent->velocity += MOVE_SPEED * strafeDirection;
            }
            if (renderSys->IsKeyReleased(GLFW_KEY_D)) {
                velocityComponent->velocity = glm::vec3(0.f);
            }
            if (renderSys->IsKeyHeldDown(GLFW_KEY_A)) {
                glm::vec3 strafeDirection = glm::cross(direction, up);
                velocityComponent->velocity += -MOVE_SPEED * strafeDirection;
            }
            if (renderSys->IsKeyReleased(GLFW_KEY_A)) {
                velocityComponent->velocity = glm::vec3(0.f);
            }
            if (renderSys->IsKeyHeldDown(GLFW_KEY_SPACE)) {
                velocityComponent->velocity.y += MOVE_SPEED;
            }
            if (renderSys->IsKeyReleased(GLFW_KEY_SPACE)) {
                velocityComponent->velocity = glm::vec3(0.f);
            }
        }

        else {
            if (renderSys->IsKeyHeldDown(GLFW_KEY_W)) {
                velocityComponent->velocity.z += MOVE_SPEED;
                transformComponent->rotation = glm::quat(glm::vec3(0.0f, glm::radians(0.0f), 0.0f));
            }
            if (renderSys->IsKeyReleased(GLFW_KEY_W)) {
                velocityComponent->velocity = glm::vec3(0.f);
            }
            if (renderSys->IsKeyHeldDown(GLFW_KEY_S)) {
                velocityComponent->velocity.z -= MOVE_SPEED;
                transformComponent->rotation = glm::quat(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
            }
            if (renderSys->IsKeyReleased(GLFW_KEY_S)) {
                velocityComponent->velocity = glm::vec3(0.f);
            }
            if (renderSys->IsKeyHeldDown(GLFW_KEY_D)) {
                velocityComponent->velocity.x -= MOVE_SPEED;
                transformComponent->rotation = glm::quat(glm::vec3(0.0f, glm::radians(-90.0f), 0.0f));
            }
            if (renderSys->IsKeyReleased(GLFW_KEY_D)) {
                velocityComponent->velocity = glm::vec3(0.f);
            }
            if (renderSys->IsKeyHeldDown(GLFW_KEY_A)) {
                velocityComponent->velocity.x += MOVE_SPEED;
                transformComponent->rotation = glm::quat(glm::vec3(0.0f, glm::radians(90.0f), 0.0f));
            }
            if (renderSys->IsKeyReleased(GLFW_KEY_A)) {
                velocityComponent->velocity = glm::vec3(0.f);
            }
            if (renderSys->IsKeyHeldDown(GLFW_KEY_SPACE)) {
                velocityComponent->velocity.y += MOVE_SPEED;
            }
            if (renderSys->IsKeyReleased(GLFW_KEY_SPACE)) {
                velocityComponent->velocity = glm::vec3(0.f);
            }
        }
        break;

    case NONE:
        // This should never come to pass
        break;
    default:
        break;
    }

    if (gameMode == GameMode::PLAYER) {
        renderSys->GetCamera()->position = transformComponent->position - glm::vec3(0, -3, 10);

        if (!renderSys->GetMouseStatus()) {
            renderSys->GetCamera()->target = transformComponent->position;
        }
    }
}

void Scene::Shutdown() 
{
    engine->Shutdown();

    engine = nullptr;
    delete engine;

    timer = nullptr;
    delete timer;
}

ECSengine* Scene::GetEngineInstance()
{
    return engine;
}

float Scene::GetDeltaTime() 
{
    return timer->GetDeltaTime();
}

// Load all the asset files
void Scene::LoadAssets()
{
    LoadLighting();
    LoadModels();
    LoadTextures();
    LoadSounds();
}

void Scene::LoadModels() 
{
    // Meshes loaded here
    renderSystem->SetMeshPath("../assets/meshes");

    renderSystem->LoadModel("skybox_sphere.ply", "skybox", ModelFormat::PLY, shaderID);
    renderSystem->LoadModel("steve.ply", "steve", ModelFormat::PLY, shaderID);
    renderSystem->LoadModel("castle_guard.fbx", "merle", ModelFormat::FBX, shaderID);
    renderSystem->LoadModel("creepyMonster.ply", "creepyMonster", ModelFormat::PLY, shaderID);
    renderSystem->LoadModel("female_warrior.ply", "female_warrior", ModelFormat::PLY, shaderID);
    renderSystem->LoadModel("plane.ply", "plane", ModelFormat::PLY, shaderID);
    renderSystem->LoadModel("imposter.ply", "flame", ModelFormat::PLY, shaderID);
    renderSystem->LoadModel("minotaur.ply", "minotaur", ModelFormat::PLY, shaderID);
    renderSystem->LoadModel("campfire.ply", "campfire", ModelFormat::PLY, shaderID);
    renderSystem->LoadModel("tree.ply", "tree", ModelFormat::PLY, shaderID);
    renderSystem->LoadModel("tree1.ply", "tree1", ModelFormat::PLY, shaderID);

}

void Scene::LoadTextures() 
{
    // Textures loaded here
    renderSystem->SetTexturePath("../assets/textures");

    // Skybox textures

    // BMP
    unsigned int skyboxTextureID0 = 0;
    std::string errorString0 = "";

    std::string skyboxNameBMP = "sunnyday";
    renderSystem->LoadCubeMapTextureBMP(skyboxTextureID0, skyboxNameBMP,
        "TropicalSunnyDayLeft2048.bmp",
        "TropicalSunnyDayRight2048.bmp",
        "TropicalSunnyDayDown2048.bmp",
        "TropicalSunnyDayUp2048.bmp",
        "TropicalSunnyDayFront2048.bmp",
        "TropicalSunnyDayBack2048.bmp",
        true, errorString0);

    // PNG
    unsigned int skyboxTextureID1 = 0;
    std::string errorString1 = "";

    std::string skyboxNamePNG1 = "desert";
    renderSystem->LoadCubeMapTexturePNG(skyboxTextureID1, skyboxNamePNG1,
        "desertlf.png",
        "desertrt.png",
        "desertdn.png",
        "desertup.png",
        "desertft.png",
        "desertbk.png",
        true, errorString1);
    
    unsigned int skyboxTextureID2 = 0;
    std::string errorString2 = "";

    std::string skyboxNamePNG2 = "orangeSky";
    renderSystem->LoadCubeMapTexturePNG(skyboxTextureID2, skyboxNamePNG2,
        "sky106lf.png",
        "sky106rt.png",
        "sky106dn.png",
        "sky106up.png",
        "sky106ft.png",
        "sky106bk.png",
        true, errorString2);

    // 2D textures

    // PNG Textures
    unsigned int textureID = 0;
    renderSystem->Load2DTexturePNG("man.png");
    renderSystem->Load2DTexturePNG("noman.png");
    renderSystem->Load2DTexturePNG("herobrinehd.png");
    renderSystem->Load2DTexturePNG("dummy.png");
    renderSystem->Load2DTexturePNG("flame.png");
    renderSystem->Load2DTexturePNG("fire-flames-texture.png");
    renderSystem->Load2DTexturePNG("Campfire_MAT_BaseColor_00.png");
    renderSystem->Load2DTexturePNG("Campfire_MAT_BaseColor_01.png");
    renderSystem->Load2DTexturePNG("Campfire_MAT_Metallic.png");
    renderSystem->Load2DTexturePNG("Campfire_MAT_Roughness.png");
    renderSystem->Load2DTexturePNG("Campfire_MAT_AO.png");
    renderSystem->Load2DTexturePNG("tree001.png");
    renderSystem->Load2DTexturePNG("tree001_SubsurfaceColor.png");
    renderSystem->Load2DTexturePNG("tree001_Gloss.png");
    renderSystem->Load2DTexturePNG("tree001_Opacity.png");

    // BMP Textures
    renderSystem->Load2DTextureBMP("seamless-green-grass-pattern.bmp");
    renderSystem->Load2DTextureBMP("full_low_body__BaseColor.bmp");
    renderSystem->Load2DTextureBMP("blue_flame.bmp");
}

void Scene::LoadSounds()
{
    // Flags
    constexpr int flags = FMOD_DEFAULT | FMOD_3D | FMOD_LOOP_NORMAL;

    // Sounds loaded here
    soundSystem->SetSoundPath("../assets/sounds/sfx");

    soundSystem->LoadSound("deep_stone_lullaby.mp3", flags);
    soundSystem->LoadSound("chicken.wav", flags);
    soundSystem->LoadSound("my_dark_disquiet.mp3", flags);
}

void Scene::LoadLighting()
{
    // attenuation on all lights
    const glm::vec4 constLightAtten = glm::vec4(0.1f, 2.5e-5f, 2.5e-6f, 1.0f);

    // The Lights
    {
        cLight* newLight = lightSystem->AddLight(glm::vec4(175, 175, 175, 1));
        newLight->diffuse = glm::vec4(1.f, 1.f, 1.f, 1.f);
        newLight->specular = glm::vec4(1.f, 1.f, 1.f, 1.f);
        newLight->direction = glm::vec4(1.f, 1.f, 1.f, 1.f);
        newLight->atten = constLightAtten;
        newLight->param1 = glm::vec4(0.f, 50.f, 50.f, 1.f);
        newLight->param2 = glm::vec4(1.f, 0.f, 0.f, 1.f);
    }

    {
        cLight* newLight = lightSystem->AddLight(glm::vec4(100.f, 10.f, 60.f, 1.f));
        newLight->diffuse = glm::vec4(100.f, 100.f, 1.f, 1.f);
        newLight->specular = glm::vec4(1.f, 1.f, 1.f, 1.f);
        newLight->direction = glm::vec4(1.f, 1.f, 1.f, 1.f);
        newLight->atten = glm::vec4(0.1f, 2.5e-5f, 2.5e-6f, 1.0f);
        newLight->param1 = glm::vec4(0.f, 50.f, 50.f, 1.f);
        newLight->param2 = glm::vec4(1.f, 0.f, 0.f, 1.f);
    }
}

void Scene::CreateFlatPlane() {

    unsigned int gridSize = 10;
    glm::vec3 position = glm::vec3(0.f);

    for (unsigned int i = 0; i < gridSize; i++) {
        for (unsigned int j = 0; j < gridSize; j++) {
            {   // Entity "plane"

                unsigned int entityID = engine->CreateEntity();

                TransformComponent* transformComponent = engine->AddComponent<TransformComponent>(entityID);
                transformComponent->position = position;
                transformComponent->scale = glm::vec3(0.1f);
                transformComponent->rotation = glm::quat(glm::vec3(0.f));

                ShaderComponent* shaderComponent = engine->AddComponent<ShaderComponent>(entityID);
                shaderComponent->shaderID = shaderID;

                MeshComponent* meshComponent = engine->AddComponent<MeshComponent>(entityID);
                meshComponent->meshName = "plane";
                meshComponent->modelFormat = ModelFormat::PLY;
                meshComponent->isWireframe = false;

                TextureComponent* textureComponent = engine->AddComponent<TextureComponent>(entityID);
                textureComponent->useTexture = true;
                textureComponent->textureFormat = TextureFormat::BMP;
                textureComponent->textures[0] = "seamless-green-grass-pattern.bmp";
                textureComponent->textureRatios[0] = 0.1f;

                LitComponent* litComponent = engine->AddComponent<LitComponent>(entityID);
                litComponent->isLit = true;
            }

            position.x += 20.f;
        }

        position.x = 0.f;
        position.z += 20.f;
    }

    //{   // Entity "tree"

    //    unsigned int entityID = engine->CreateEntity();

    //    TransformComponent* transformComponent = engine->AddComponent<TransformComponent>(entityID);
    //    transformComponent->position = glm::vec3(100.f, 1.9f, 20.f);
    //    transformComponent->scale = glm::vec3(2.f);
    //    transformComponent->rotation = glm::quat(glm::vec3(0.f, 0.f, 0.f));

    //    ShaderComponent* shaderComponent = engine->AddComponent<ShaderComponent>(entityID);
    //    shaderComponent->shaderID = shaderID;

    //    MeshComponent* meshComponent = engine->AddComponent<MeshComponent>(entityID);
    //    meshComponent->meshName = "tree1";
    //    meshComponent->modelFormat = ModelFormat::PLY;

    //    RigidBodyComponent* rigidBodyComponent = engine->AddComponent<RigidBodyComponent>(entityID);
    //    rigidBodyComponent->bodyShape = new physics::CylinderShape(glm::vec3(1.f));
    //    rigidBodyComponent->rigidBodyDesc.isStatic = true;
    //    rigidBodyComponent->rigidBodyDesc.useInertia = false;
    //    rigidBodyComponent->usePhysics = true;

    //    TextureComponent* textureComponent = engine->AddComponent<TextureComponent>(entityID);
    //    textureComponent->textureFormat = TextureFormat::PNG;
    //    textureComponent->useTexture = true;
    //    textureComponent->textures[0] = "tree001.png";
    //    textureComponent->textureRatios[0] = 0.45f;
    //    textureComponent->textures[1] = "tree001_SubsurfaceColor.png";
    //    textureComponent->textureRatios[1] = 0.5f;
    //    textureComponent->textures[2] = "tree001_Gloss.png";
    //    textureComponent->textureRatios[2] = 0.01f;
    //    textureComponent->textures[3] = "tree001_Opacity.png";
    //    textureComponent->textureRatios[3] = 1.f;

    //    LitComponent* litComponent = engine->AddComponent<LitComponent>(entityID);
    //    litComponent->isLit = true;
    //}
    //
    //{   // Entity "tree"

    //    unsigned int entityID = engine->CreateEntity();

    //    TransformComponent* transformComponent = engine->AddComponent<TransformComponent>(entityID);
    //    transformComponent->position = glm::vec3(80.f, 1.9f, 60.f);
    //    transformComponent->scale = glm::vec3(2.f);
    //    transformComponent->rotation = glm::quat(glm::vec3(0.f, 0.f, 0.f));

    //    ShaderComponent* shaderComponent = engine->AddComponent<ShaderComponent>(entityID);
    //    shaderComponent->shaderID = shaderID;

    //    MeshComponent* meshComponent = engine->AddComponent<MeshComponent>(entityID);
    //    meshComponent->meshName = "tree1";
    //    meshComponent->modelFormat = ModelFormat::PLY;

    //    RigidBodyComponent* rigidBodyComponent = engine->AddComponent<RigidBodyComponent>(entityID);
    //    rigidBodyComponent->bodyShape = new physics::CylinderShape(glm::vec3(1.f));
    //    rigidBodyComponent->rigidBodyDesc.isStatic = true;
    //    rigidBodyComponent->rigidBodyDesc.useInertia = false;
    //    rigidBodyComponent->usePhysics = true;

    //    TextureComponent* textureComponent = engine->AddComponent<TextureComponent>(entityID);
    //    textureComponent->textureFormat = TextureFormat::PNG;
    //    textureComponent->useTexture = true;
    //    textureComponent->textures[0] = "tree001.png";
    //    textureComponent->textureRatios[0] = 0.45f;
    //    textureComponent->textures[1] = "tree001_SubsurfaceColor.png";
    //    textureComponent->textureRatios[1] = 0.5f;
    //    textureComponent->textures[2] = "tree001_Gloss.png";
    //    textureComponent->textureRatios[2] = 0.01f;
    //    textureComponent->textures[3] = "tree001_Opacity.png";
    //    textureComponent->textureRatios[3] = 1.f;

    //    LitComponent* litComponent = engine->AddComponent<LitComponent>(entityID);
    //    litComponent->isLit = true;
    //}
    //
    //{   // Entity "tree"

    //    unsigned int entityID = engine->CreateEntity();

    //    TransformComponent* transformComponent = engine->AddComponent<TransformComponent>(entityID);
    //    transformComponent->position = glm::vec3(20.f, 1.9f, 40.f);
    //    transformComponent->scale = glm::vec3(2.f);
    //    transformComponent->rotation = glm::quat(glm::vec3(0.f, 0.f, 0.f));

    //    ShaderComponent* shaderComponent = engine->AddComponent<ShaderComponent>(entityID);
    //    shaderComponent->shaderID = shaderID;

    //    MeshComponent* meshComponent = engine->AddComponent<MeshComponent>(entityID);
    //    meshComponent->meshName = "tree1";
    //    meshComponent->modelFormat = ModelFormat::PLY;

    //    RigidBodyComponent* rigidBodyComponent = engine->AddComponent<RigidBodyComponent>(entityID);
    //    rigidBodyComponent->bodyShape = new physics::CylinderShape(glm::vec3(1.f));
    //    rigidBodyComponent->rigidBodyDesc.isStatic = true;
    //    rigidBodyComponent->rigidBodyDesc.useInertia = false;
    //    rigidBodyComponent->usePhysics = true;

    //    TextureComponent* textureComponent = engine->AddComponent<TextureComponent>(entityID);
    //    textureComponent->textureFormat = TextureFormat::PNG;
    //    textureComponent->useTexture = true;
    //    textureComponent->textures[0] = "tree001.png";
    //    textureComponent->textureRatios[0] = 0.45f;
    //    textureComponent->textures[1] = "tree001_SubsurfaceColor.png";
    //    textureComponent->textureRatios[1] = 0.5f;
    //    textureComponent->textures[2] = "tree001_Gloss.png";
    //    textureComponent->textureRatios[2] = 0.01f;
    //    textureComponent->textures[3] = "tree001_Opacity.png";
    //    textureComponent->textureRatios[3] = 1.f;

    //    LitComponent* litComponent = engine->AddComponent<LitComponent>(entityID);
    //    litComponent->isLit = true;
    //}
}