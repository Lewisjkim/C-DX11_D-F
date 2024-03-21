#include "pch.h"
#include "CBakalAttackScript.h"

CBakalAttackScript::CBakalAttackScript() :
	CScript((UINT)SCRIPT_TYPE::BAKALATTACKSCRIPT),
	m_Damage(100)
{
}

CBakalAttackScript::~CBakalAttackScript()
{
}

void CBakalAttackScript::begin()
{
	SetLifeSpan(2.f);
}

void CBakalAttackScript::tick()
{
	
	 vector<CGameObject*> vecMonster = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(3)->GetParentObject();
	 for (size_t i = 0; i < vecMonster.size(); i++)
	 {
		 if (vecMonster[i]->GetName() == L"Bakal") 
		 {
			 BakalRot = vecMonster[i]->Transform()->GetRelativeRot();
			 BakalPos = vecMonster[i]->Transform()->GetRelativePos();
		 }
	 }
	 Transform()->SetRelativeRot(BakalRot);
	 if (BakalRot == Vec3(0.f, 0.f, 0.f))
	 {
		 Transform()->SetRelativePos(BakalPos + Vec3(120.f, 0.f, 0.f));
	 }
	 else
	 {
		 Transform()->SetRelativePos(BakalPos + Vec3(-120.f, 0.f, 0.f));
	 }
	 
	
}
