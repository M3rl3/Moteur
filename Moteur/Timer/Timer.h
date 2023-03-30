#ifndef _TIMER_H_
#define _TIMER_H_

#include <chrono>

class CTimer
{
private:
	static CTimer* m_pInstance;

public:
	static CTimer* GetInstance();
private:
	void DestroyInstance();

private:
	std::chrono::steady_clock::time_point	m_TimeLastUpdate;
	std::chrono::steady_clock::time_point	m_TimeCurrentUpdate;
	float									m_fCurrentTime;
	float									m_fTimeDelta;
	float									m_fTimeDefault;
	float									m_fFrameRate;

private:
	explicit CTimer();
	~CTimer();
	void Destroy();

public:
	void Update();

public:
	float GetTimeDelta()		{ return m_fTimeDelta > 0.1 ? 0 : m_fTimeDelta; }
	float GetTimeDefault()		{ return m_fTimeDefault; }
	void SetFrameRate(int frameRate);
	bool IsUpdateAvailable();
};

#endif //_TIMER_H_