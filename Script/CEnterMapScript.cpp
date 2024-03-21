#include "pch.h"
#include "CEnterMapScript.h"
#include <Engine/CEventMgr.h>
#include"/DirectX_Practice/DirectX_/Project/Client/CLevelSaveLoad.h"

CEnterMapScript::CEnterMapScript()		:
	CScript((UINT)SCRIPT_TYPE::ENTERMAPSCRIPT),
	m_time(0.f)
{
}

CEnterMapScript::~CEnterMapScript()
{
}

void CEnterMapScript::tick()
{
	m_time += DT;


	if (m_time > 4.f)
	{
		CLevel* pNewLevel = CLevelSaveLoad::Stop(L"Level\\stage1_1.lv", LEVEL_STATE::STOP);
		pNewLevel->SetName(L"stage1_1");

		tEvent evn = {};
		evn.Type = EVENT_TYPE::LEVEL_CHANGE;
		evn.wParam = (DWORD_PTR)pNewLevel;

		CEventMgr::GetInst()->AddEvent(evn);

	}
}
