#include "SoundSystem.h"

#include "../Global.h"

#include "../Components/TransformComponent.h"
#include "../Components/SoundComponent.h"

#include <iostream>

// Constructor
SoundSystem::SoundSystem() {
	soundManager = new cSoundManager();
}

// Destructor
SoundSystem::~SoundSystem() {

}

// Creates a channel group named what was passed in
void SoundSystem::CreateChannelGroup(std::string channel_group_name)
{
	soundManager->CreateChannelGroup(channel_group_name);
}

// Creates 3 default channel groups (master, sounds, sfx)
void SoundSystem::CreateDefaultChannelGroups(std::string channel_group_name)
{
	soundManager->CreateChannelGroup("master");
	soundManager->CreateChannelGroup("sounds");
	soundManager->CreateChannelGroup("sfx");
	
	soundManager->SetParentChannel("sounds", "master");
	soundManager->SetParentChannel("sfx", "master");
	soundManager->SetChannelGroupVolume("sounds", 0.5f);
	soundManager->SetChannelGroupVolume("sfx", 0.5f);
}

void SoundSystem::SetSoundPath(std::string filePath)
{
	std::cout << "\nLoading sounds..." << std::endl;
	soundManager->SetBasePath(filePath);
}

// Method for loading sounds from sound files (mp3/wav)
bool SoundSystem::LoadSound(const std::string& path, const std::string& sound_name, const int mode)
{
	if (soundManager->LoadSounds(sound_name, path, mode)) {
		std::cout << "Model " << sound_name << " loaded successfully." << std::endl;
		return true;
	}
	else {
		std::cout << "Could not load sound " << sound_name << " into FMOD." << std::endl;
		return false;
	}
}

bool SoundSystem::LoadSound(const std::string& sound_name, const int mode)
{
	if (soundManager->LoadSounds(sound_name, mode)) {
		std::cout << "Sound " << sound_name << " loaded successfully." << std::endl;
		return true;
	}
	else {
		std::cout << "Could not load sound " << sound_name << " into FMOD." << std::endl;
		return false;
	}
}

// Method for playing sounds in a 3D environment
void SoundSystem::PlaySound(const std::string& sound_name, glm::vec3 position, float max_distance, FMOD::Channel** channel)
{
	soundManager->PlaySounds(sound_name, position, max_distance, channel);
}

// Method for playing sounds on a channel group
void SoundSystem::PlaySound(const std::string& sound_name, const std::string& channel_group_name)
{
	soundManager->PlaySounds(sound_name, channel_group_name);
}

void SoundSystem::StopAllSounds()
{
	cSoundManager::ChannelGroup* channelGroup0;
	soundManager->FetchChannelGroup("master", &channelGroup0);

	cSoundManager::ChannelGroup* channelGroup1;
	soundManager->FetchChannelGroup("sounds", &channelGroup1);

	cSoundManager::ChannelGroup* channelGroup2;
	soundManager->FetchChannelGroup("sfx", &channelGroup2);

	channelGroup1->current_grp->stop();
	channelGroup2->current_grp->stop();
	channelGroup0->current_grp->stop();
}

// The update method called every tick
void SoundSystem::Process(const std::vector<Entity*>& entities, float dt) {

	// Update the position of the listener
	soundManager->Tick(cam->position);

	// Make a copy of all the entity components
	SoundComponent* soundComponent = nullptr;
	TransformComponent* transformComponent = nullptr;

	// Iterate through all entities
	for (int i = 0; i < entities.size(); i++) {

		Entity* currentEntity = entities[i];

		// get the specific instances for all components
		transformComponent = currentEntity->GetComponentByType<TransformComponent>();
		soundComponent = currentEntity->GetComponentByType<SoundComponent>();

		// check if the instances actually exist
		if (soundComponent != nullptr && transformComponent != nullptr) {

			if (soundComponent->isPlaying && soundComponent->doOnce) {
				if (!soundManager->PlaySounds(soundComponent->soundName,
					transformComponent->position,
					soundComponent->maxDistance,
					&soundComponent->attachedSound))
				{
					std::cout << "\nSound "<< soundComponent->soundName << " not found." << std::endl;
				}

				soundComponent->doOnce = false;
			}
			else if (soundComponent->isPlaying) {
				// Update the positions of all the attached sounds
				soundManager->UpdateSoundPosition(soundComponent->attachedSound, transformComponent->position);

				// Update sound volume
				soundManager->UpdateVolume(soundComponent->attachedSound, soundComponent->soundVolume);
			}
			else {
				continue;
			}
		}	
	}
}

void SoundSystem::Shutdown()
{
	soundManager = nullptr;
	delete soundManager;
}

cSoundManager* SoundSystem::GetSoundManager()
{
	return soundManager;
}