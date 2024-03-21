#pragma once

class CEngine
	: public CSingleton<CEngine>
{
private:
	HWND	m_hWnd;
	wstring	gameid;
	//윈도우 해상도
	Vec2	m_vResolution; //vector Resolution

public:
	Vec2 GetWindowResolution() { return m_vResolution; }
	HWND GetMainWnd() { return m_hWnd; }

public:
	int init(HWND _hWnd, UINT _iWidth, UINT _iHeight);
	void progress();
	wstring GetGameID() { return gameid; }
private:
	void tick();
	void render();

public:
	CEngine();
	~CEngine();
};
