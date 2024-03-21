#include "pch.h"
#include "CBakalStampScript.h"

CBakalStampScript::CBakalStampScript()		:
	CScript((UINT)SCRIPT_TYPE::BAKALSTAMPSCRIPT)
{
}

CBakalStampScript::~CBakalStampScript()
{
}

void CBakalStampScript::begin()
{
}

void CBakalStampScript::tick()
{//dust오브젝트에 스크립트 추가해두었고
	//바칼 오브젝트의 위치를 찾아서
	vector<CGameObject*> vecMon = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(3)->GetParentObject();
	for (size_t i = 0; i < vecMon.size(); i++)
	{
		if (vecMon[i]->GetName() == L"Bakal") 
		{
			m_Pos = vecMon[i]->Transform()->GetRelativePos();//바칼 pos
			m_Rot = vecMon[i]->Transform()->GetRelativeRot();//바칼 rot
			if (GetOwner()->GetName() == L"BakalStampDust")
			{
				if (m_Rot == Vec3(0.f, 0.f, 0.f))//오른쪽
				{
					Transform()->SetRelativePos(m_Pos + Vec3(100.f, -80.f, 0.f));
				}
				else if(m_Rot == Vec3(0.f,-3.14159274f,0.f))//왼쪽
				{
 			 		Transform()->SetRelativePos(m_Pos + Vec3(-100.f, -80.f, 0.f));
				}
			}
			else if (GetOwner()->GetName() == L"BakalMeteo"&&
						GetOwner()->GetName() == L"BakalMeteo2"&&
						GetOwner()->GetName() == L"BakalMeteo3")
			{
				if (m_Rot == Vec3(0.f, 0.f, 0.f))
				{
					Transform()->SetRelativePos(m_Pos + Vec3(100.f, 500.f, 0.f));
				}
				else if (m_Rot == Vec3(0.f, -3.14159274f, 0.f))//왼쪽
				{
					Transform()->SetRelativePos(m_Pos + Vec3(-100.f, 500.f, 0.f));
				}
			}
		}
	}

	//Transform()->SetRelativeRot(m_Rot);
}
