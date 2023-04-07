#include "Timer.h"

Timer::Timer()
	: currentTime(0.f), deltaTime(0.f), elapsedTime(0.f), frameRate(0.f)
{
	timeLastUpdate = std::chrono::steady_clock::now();
	timeCurrentUpdate = std::chrono::steady_clock::now();
}

Timer::~Timer()
{

}

void Timer::Update()
{
	timeLastUpdate = timeCurrentUpdate;
	timeCurrentUpdate = std::chrono::steady_clock::now();

	std::chrono::duration<float> elapsed = timeCurrentUpdate - timeLastUpdate;
	elapsedTime = elapsed.count();
	currentTime += elapsedTime;
}

void Timer::SetFrameRate(int FrameRate)
{
	frameRate = 1.f / FrameRate;
}

bool Timer::IsUpdateAvailable()
{
	if (currentTime > frameRate)
	{
		deltaTime = currentTime;
		currentTime = 0.f;
		return true;
	}

	return false;
}

float Timer::GetDeltaTime()
{ 
	return deltaTime > 0.1 ? 0 : deltaTime; 
}

float Timer::GetElapsedTime()
{
	return elapsedTime;
}