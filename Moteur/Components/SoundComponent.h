#include <ECS/Component.h>

#include <FMOD/fmod.hpp>

#include <string>

// Component holding all sounds playing from entities
struct SoundComponent : Component {
public:
	// Constructor
	SoundComponent() :
		componentType("SoundComponent"),
		attachedSound(nullptr),
		soundName(""),
		maxDistance(1.f),
		soundVolume(1.f),
		isPlaying(true),
		doOnce(true)
	{};

	// Destructor
	~SoundComponent() {};

	// Sound attached to the entity
	FMOD::Channel* attachedSound;

	// Sound attached to the entity
	std::string soundName;

	// Sound volume
	float soundVolume;

	// sound attenuation
	float maxDistance;

	// Is the sound playing?
	bool isPlaying;

	// Don't want the sound playing an infinite amount of times
	bool doOnce;

private:
	// name of the component
	std::string componentType;
};