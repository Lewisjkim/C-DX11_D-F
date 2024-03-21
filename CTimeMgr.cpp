#include "pch.h"
#include "CTimeMgr.h"
#include "CFontMgr.h"
#include "CKeyMgr.h"
#include "CEngine.h"
#include <windowsx.h>


CTimeMgr::CTimeMgr()
	: m_llPrevCount{}
	, m_llCurCount{}
	, m_llFrequency{}
	, m_iCallCount(0)
	, m_fDeltaTime(0.f)
	, m_fTime(0.f)
{

}

CTimeMgr::~CTimeMgr()
{

}



void CTimeMgr::init()
{
	// 1�ʴ� ī���� ������
	QueryPerformanceFrequency(&m_llFrequency);

	QueryPerformanceCounter(&m_llCurCount);
	QueryPerformanceCounter(&m_llPrevCount);
}

void CTimeMgr::tick()
{
	QueryPerformanceCounter(&m_llCurCount);

	// tick ���� �ð�
	m_fDeltaTime = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (float)m_llFrequency.QuadPart;

	// ���� �ð�
	m_fTime += m_fDeltaTime;

	// �Լ� ȣ�� Ƚ��
	++m_iCallCount;

	// ���� ī��Ʈ ���� ���� ī��Ʈ�� ����
	m_llPrevCount = m_llCurCount;

	// GlobalData ����
	GlobalData.tDT = m_fDeltaTime;
	GlobalData.tAccTime += m_fDeltaTime;
}

void CTimeMgr::render()
{
	static wchar_t szBuff[256] = {};
	static wchar_t szBuffmousepos[256] = {};
	HWND m_hwnd = CEngine::GetInst()->GetMainWnd();
	POINT mousePos;
	Vec2 resolution = CEngine::GetInst()->GetWindowResolution();
	resolution = resolution / 2;
	GetCursorPos(&mousePos);
	ScreenToClient(m_hwnd, &mousePos);
	mousePos.x -= resolution.x;
	mousePos.y -= resolution.y;
	if(mousePos.y<0)
		mousePos.y = abs(mousePos.y);
	else
		mousePos.y = -abs(mousePos.y);
	swprintf_s(szBuffmousepos, L"MousX: %d, MouseY : %d", mousePos.x, mousePos.y);
	// 1�ʿ� �ѹ�
	if (1.f <= m_fTime)
	{
		swprintf_s(szBuff, L"FPS : %d, DT : %f", m_iCallCount, m_fDeltaTime);
		//SetWindowText(CEngine::GetInst()->GetMainWnd(), szBuff);

		m_fTime = 0.f;
		m_iCallCount = 0;
	}
	CFontMgr::GetInst()->DrawFont(szBuff, 10, 20, 16, FONT_RGBA(255, 0, 0, 255));
	CFontMgr::GetInst()->DrawFont(szBuffmousepos, 10, 40, 16, FONT_RGBA(255, 0, 0, 255));
}
