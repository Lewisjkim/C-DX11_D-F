#include "pch.h"
#include "CBreakObject.h"

CBreakObject::CBreakObject()	:
	CScript((UINT)SCRIPT_TYPE::BREAKOBJECT)
{
}

CBreakObject::~CBreakObject()
{
}

void CBreakObject::tick()
{
}

void CBreakObject::BeginOverlap(CCollider2D* _Other)
{
	if (L"bullet" == _Other->GetOwner()->GetName())
	{
		Vec3 ObjPos = Transform()->GetRelativePos();
		ShowBObjectParticle(L"texture\\Prefab\\breakobject1.prefab", ObjPos + Vec3(-20.f,0.f,0.f));
		ShowBObjectParticle(L"texture\\Prefab\\breakobject2.prefab", ObjPos + Vec3(0.f, 0.f, 0.f));
		ShowBObjectParticle(L"texture\\Prefab\\breakobject3.prefab", ObjPos + Vec3(20.f, 0.f, 0.f));
		Ptr<CSound> breakobj = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunner\\hit_wood_01.ogg");
		breakobj->Play(1, 0.3f, false);
		//SetLifeSpan(2.f);
		Destroy();
	}
}

void CBreakObject::ShowBObjectParticle(wstring relativpath, Vec3 _vPos)
{
	SpawnPrefab(relativpath, _vPos, 1.f);

	
}
