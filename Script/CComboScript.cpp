#include "pch.h"
#include "CComboScript.h"
#include "CPlayerScript.h"
#include "CCameraMoveScript.h"

CComboScript::CComboScript()		:
	CScript((UINT)SCRIPT_TYPE::COMBOSCRIPT)
{
	
}

CComboScript::~CComboScript()
{
}

void CComboScript::begin()
{
	m_time = 0.f;
	m_resettime = 0.f;
}

void CComboScript::tick()
{
	m_time += DT;
	m_resettime += DT;
	vector<CGameObject*> vecCam = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(10)->GetParentObject();
	CCameraMoveScript* MainCamScript = vecCam[0]->GetScript<CCameraMoveScript>();
	vector<CGameObject*> vecplayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
	CPlayerScript* plScript = vecplayer[0]->GetScript<CPlayerScript>();

	mainCam = MainCamScript->GetOwner();
	Vec3 CamPos = mainCam->Transform()->GetRelativePos();
	
	if (GetOwner()->GetName() == L"combo")
	{
		Vec3 ComboPos = Transform()->GetRelativePos();
		ComboPos = CamPos + Vec3(-500.f, 0.f, 0.f);

		Transform()->SetRelativePos(ComboPos);

		int combo = plScript->GetComboCount();

		if (m_resettime > 4.f)
		{
			int combo1 = plScript->GetComboCount();
			if (combo == combo1)
				plScript->ComboReset();

			m_resettime = 0.f;
		}
		if (m_time > 0.5f)
		{
			if (combo != 0)
			{
				int count = GetDigitCount(combo);
				ShowCombo(combo, count, mainCam);
				m_time = 0.f;
			}
		}
	}
	//else if (GetOwner()->GetName() == L"c1" || GetOwner()->GetName() == L"c2" ||GetOwner()->GetName() == L"c3" || GetOwner()->GetName() == L"c4" ||
	//	GetOwner()->GetName() == L"c5" || GetOwner()->GetName() == L"c6" ||GetOwner()->GetName() == L"c7" || GetOwner()->GetName() == L"c8" ||
	//	GetOwner()->GetName() == L"c9" || GetOwner()->GetName() == L"c0")
	//{
	//	
	//	/*Vec3 curPos = Transform()->GetRelativePos();
	//	Transform()->SetRelativePos(CamPos);*/
	//}
		
		
}