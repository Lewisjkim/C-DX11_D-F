#pragma once
#include <ctime>

class CTimeMgr
	: public CSingleton<CTimeMgr>
{	
	SINGLE(CTimeMgr)
private:
	LARGE_INTEGER	m_llPrevCount;
	LARGE_INTEGER	m_llCurCount;
	LARGE_INTEGER	m_llFrequency;

	UINT			m_iCallCount;
	UINT			m_FPS;
	float			m_fDeltaTime;
	float			m_fTime;
	float			m_fTimeScale;
	float			wait;
public:
	void init();
	void tick();
	void render();
	void Wait(float _time) 
	{
		wait = clock() + _time * CLOCKS_PER_SEC;
		while (clock() < wait){}
	}
public:
	float GetDeltaTime() { return m_fDeltaTime; }
	UINT GetFPS() { return m_FPS; }
};

