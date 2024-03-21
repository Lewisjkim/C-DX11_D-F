#include "pch.h"
#include "CGTStoneScript.h"

CGTStoneScript::CGTStoneScript() :
	CScript((UINT)SCRIPT_TYPE::GTSTONESCRIPT),
	m_Damage(7),
	m_time(0.f),
	m_Speed(200.f)
{
}

CGTStoneScript::~CGTStoneScript()
{
}

void CGTStoneScript::begin()
{
	SetLifeSpan(1.5f);
	vector<CGameObject*> vecPlayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
	Vec3 playerPos = vecPlayer[0]->Transform()->GetRelativePos();
	playerPos = playerPos - Vec3(0.f, 10.f, 0.f);
	stonePos = Transform()->GetRelativePos();
	dirVector = (playerPos - stonePos).Normalize();
}

void CGTStoneScript::tick()
{
	stonePos += DT * dirVector * m_Speed;
	
	Transform()->SetRelativePos(stonePos);
}

void CGTStoneScript::BeginOverlap(CCollider2D* _Other)
{
}

void CGTStoneScript::OnOverlap(CCollider2D* _Other)
{
}

void CGTStoneScript::EndOverlap(CCollider2D* _Other)
{
}
