#include "Timer.h"


using namespace std;

CTimer* CTimer::m_pInstance = nullptr;
CTimer* CTimer::GetInstance()
{
	if (nullptr == m_pInstance)
		m_pInstance = new CTimer;
	return m_pInstance;
}

void CTimer::DestroyInstance()
{
	if (nullptr != m_pInstance)
	{
		m_pInstance->Destroy();
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

CTimer::CTimer()
	: m_fCurrentTime(0.f), m_fTimeDelta(0.f), m_fTimeDefault(0.f), m_fFrameRate(0.f)
{
	m_TimeLastUpdate = chrono::steady_clock::now();
	m_TimeCurrentUpdate = chrono::steady_clock::now();
}

CTimer::~CTimer()
{
}

void CTimer::Destroy()
{
}

void CTimer::Update()
{
	m_TimeLastUpdate = m_TimeCurrentUpdate;
	m_TimeCurrentUpdate = chrono::steady_clock::now();

	chrono::duration<float> elapsed = m_TimeCurrentUpdate - m_TimeLastUpdate;
	m_fTimeDefault = elapsed.count();
	m_fCurrentTime += m_fTimeDefault;
}

void CTimer::SetFrameRate(int frameRate)
{
	m_fFrameRate = 1.f / frameRate;
}

bool CTimer::IsUpdateAvailable()
{
	if (m_fCurrentTime > m_fFrameRate)
	{
		m_fTimeDelta = m_fCurrentTime;
		m_fCurrentTime = 0.f;
		return true;
	}

	return false;
}
