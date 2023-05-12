#pragma once

#include <ECS/Component.h>
#include <string>

// Component that declares the entity having this the "player".
// Only one of these might exist at any given point.
struct PlayerComponent : Component {
public:
	// Constructor
	PlayerComponent() :
		ComponentType("PlayerComponent"),
		isPlayer(true),
		isControllable(true),
		health(1000)
	{};

	// Destructor
	~PlayerComponent() {};

	// A boolean value to indicate that this is in fact the player
	bool isPlayer;

	// If the player can be controlled thru user inputs
	bool isControllable;

	// Health points of the player
	float health;

private:
	// The component type
	std::string ComponentType;
};