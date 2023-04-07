#pragma once

#include <chrono>

class Timer
{
public:

	// Constructor
	Timer();
	// Destructor
	~Timer();

	void Update();

	void SetFrameRate(int frameRate);
	bool IsUpdateAvailable();

	// Getters
	float GetDeltaTime();
	float GetElapsedTime();

private:
	// Member Variables
	std::chrono::steady_clock::time_point timeLastUpdate;
	std::chrono::steady_clock::time_point timeCurrentUpdate;

	float currentTime;
	float deltaTime;
	float elapsedTime;
	float frameRate;
};