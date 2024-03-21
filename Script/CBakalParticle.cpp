#include "pch.h"
#include "CBakalParticle.h"

CBakalParticle::CBakalParticle() :
	CScript((UINT)SCRIPT_TYPE::BAKALPARTICLE)
{
}

CBakalParticle::~CBakalParticle()
{
}

void CBakalParticle::begin()
{
}

void CBakalParticle::tick()
{
	vector<CGameObject*> vecMon = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(3)->GetParentObject();
	for (size_t i = 0; i < vecMon.size(); i++)
	{
		if (vecMon[i]->GetName() == L"Bakal") 
		{
			BakalPos = vecMon[i]->Transform()->GetRelativePos();
			BakalPos = BakalPos + Vec3(0.f, 10.f, 90.f);
		}
		else if (vecMon[i]->GetName() != L"Bakal")
		{
			DeleteObject(this->GetOwner());
		}
	}

	

	Transform()->SetRelativePos(BakalPos);
}
