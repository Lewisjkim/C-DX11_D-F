#include "pch.h"
#include "CGTStoneScript2.h"

CGTStoneScript2::CGTStoneScript2()		:
	CScript((UINT)SCRIPT_TYPE::GTSTONESCRIPT2),
	m_Damage(7),
	m_time(0.f),
	m_Speed(200.f)
{
}

CGTStoneScript2::~CGTStoneScript2()
{
}

void CGTStoneScript2::begin()
{
	SetLifeSpan(1.5f);
	vector<CGameObject*> vecPlayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
	Vec3 playerPos = vecPlayer[0]->Transform()->GetRelativePos();
	playerPos = playerPos - Vec3(0.f, 10.f, 0.f);
	stonePos = Transform()->GetRelativePos();
	dirVector = (playerPos + Vec3(0.f,5.f,0.f) - stonePos).Normalize();
	onemore = true;
}

void CGTStoneScript2::tick()
{
	
	stonePos += DT * dirVector * m_Speed;
	Transform()->SetRelativePos(stonePos);
}

void CGTStoneScript2::BeginOverlap(CCollider2D* _Other)
{
}

void CGTStoneScript2::OnOverlap(CCollider2D* _Other)
{
}

void CGTStoneScript2::EndOverlap(CCollider2D* _Other)
{
}

