#pragma once

#include <ECS/System.h>

#include <string>
#include <glm/vec3.hpp>

#include "../cSoundManager/cSoundManager.h"

// The system that handles playing all the sounds
class SoundSystem : public System {
public:
	// Constructor
	SoundSystem();

	// Destructor
	~SoundSystem();

	// Creates a channel group named what was passed in
	void CreateChannelGroup(std::string channel_group_name);
	
	// Creates 3 default channel groups (master, sounds, sfx)
	void CreateDefaultChannelGroups(std::string channel_group_name);

	// Set the path where the textures are located
	void SetSoundPath(std::string filePath);

	// Method for loading sounds from sound files (mp3/wav)
	bool LoadSound(const std::string& path, const std::string& sound_name, const int mode);
	
	// Method for loading sounds from sound files (mp3/wav)
	bool LoadSound(const std::string& sound_name, const int mode);

	// Method for playing sounds in a 3D environment
	void PlaySound(const std::string& sound_name, glm::vec3 position, float max_distance, FMOD::Channel** channel);

	// Method for playing sounds on a channel group
	void PlaySound(const std::string& sound_name, const std::string& channel_group_name);

	// Stop all playing sounds
	void StopAllSounds();

	// The update method called every tick
	void Process(const std::vector<Entity*>& entities, float dt);

	// Shutdown method
	void Shutdown();

	// Getter for the sound manager object
	cSoundManager* GetSoundManager();

private:
	// Instance of the sound manager
	cSoundManager* soundManager;
};