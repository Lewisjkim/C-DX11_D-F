#include "pch.h"
#include "CLoadingCirlce.h"
#include <Engine/CEventMgr.h>
#include"/DirectX_Practice/DirectX_/Project/Client/CLevelSaveLoad.h"

CLoadingCirlce::CLoadingCirlce() :
	CScript((UINT)SCRIPT_TYPE::LOADINGCIRLCE),
	m_time(0.f)
{
	
}

CLoadingCirlce::~CLoadingCirlce()
{
}

void CLoadingCirlce::tick()
{
	m_time += DT;
	Vec3 circleRot = Transform()->GetRelativeRot();
	circleRot.z += DT * -8.f;
	Transform()->SetRelativeRot(circleRot);

	if (m_time > 4.f)
	{
		CLevel* pNewLevel = CLevelSaveLoad::Stop(L"Level\\CharacterSelect.lv", LEVEL_STATE::STOP);
		pNewLevel->SetName(L"CharacterSelect");
		tEvent evn = {};
		evn.Type = EVENT_TYPE::LEVEL_CHANGE;
		evn.wParam = (DWORD_PTR)pNewLevel;

		CEventMgr::GetInst()->AddEvent(evn);
	
	}
}
