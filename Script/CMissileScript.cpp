#include "pch.h"
#include "CMissileScript.h"
#include "CPlayerScript.h"

#include <Engine/CTimeMgr.h>

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/CEventMgr.h>

CMissileScript::CMissileScript()
	: CScript((UINT)SCRIPT_TYPE::MISSILESCRIPT),
	m_vDir(Vec3(1.f, 0.f, 0.f)),
	m_fSpeed(700.f),
	m_time(0.f)
{
}

CMissileScript::~CMissileScript()
{
}

void CMissileScript::tick()
{
	//bullet의 처음 시작하는 위치를 player위치 + alpha로 지정
	if (m_time == 0.f)
	{
		Transform()->SetRelativePos(GetPlayerPos() + Vec3(50.f, 50.f, 0.f));

		vector<CGameObject*> vecObj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
		for (size_t i = 0; i < vecObj.size(); i++)
		{
			b_dir = vecObj[i]->GetScript<CPlayerScript>()->GetDir();
		}
	}
	m_time += DT;
	 vPos = Transform()->GetRelativePos();//시작지점
	 
	 if (true == b_dir)
	 {
		 vPos += m_vDir * DT * m_fSpeed;
	 }
	 else
	 {
		 vPos -= m_vDir * DT * m_fSpeed;
	 }
	
		
	Transform()->SetRelativePos(vPos);
	if (0.8f <= m_time)
	{
		DestroyObject(this->GetOwner());
		//m_time = 0.1f;
	}
		
}

void CMissileScript::BeginOverlap(CCollider2D* _Other)
{
	if (3 == _Other->GetOwner()->GetLayerIndex())
	{
		DestroyObject(GetOwner());
	}
	else if (8 == _Other->GetOwner()->GetLayerIndex())
	{
		DestroyObject(GetOwner());
	}
}
