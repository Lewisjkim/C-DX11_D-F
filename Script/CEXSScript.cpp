#include "pch.h"
#include "CEXSScript.h"
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include "CGoblinScript.h"
#include "CPlayerScript.h"
#include "EXSBullet.h"

CEXSScript::CEXSScript() :
	CScript((UINT)SCRIPT_TYPE::EXSSCRIPT),
	Speed(100.f),
	CastingTime(0.5f),
	CoolTime(3.f),
	DurationTime(6.f),
	ExplodeRange(1.69f),
	isexplode(false)
{
	b_dir = false;
	vector<CGameObject*> vecObj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
	for (size_t i = 0; i < vecObj.size(); i++)
	{
		b_dir = vecObj[i]->GetScript<CPlayerScript>()->GetDir();
	}
}

CEXSScript::~CEXSScript()
{
}

void CEXSScript::begin()
{
	SetLifeSpan(DurationTime);
	leftcount = 0;
	rightcount = 0;
	lresttime = 0.f;
	rresttime = 0.f;
}

void CEXSScript::tick()
{
	
	time += DT;
	if (0 < Lv && Lv < 3)
	{
		MpUse = 72;
	}
	else if (2 < Lv && Lv < 5)
	{
		MpUse = 79;
	}
	else if (4 < Lv)
	{
		MpUse = 86;
	}
	Vec3 EXSPos = GetOwner()->Transform()->GetRelativePos();
	if (b_dir == false)
	{
		Vec3 rot = Vec3(0.f, -180.f, 0.f);
		rot = (rot / 180.f) * XM_PI;
		GetOwner()->Transform()->SetRelativeRot(rot);
		
		if (0.2f < time)
		{
			leftcount++;
			if (leftcount != 6 && leftcount != 7 && leftcount != 13 && leftcount != 14)
			{
				
				CGameObject* Fir1 = SpawnPrefab(L"texture\\Prefab\\EXSFire1.prefab", EXSPos + Vec3(-120.f, 20.f, 0.f));
				Fir1->SetLifeSpan(0.1f);
				Fir1->Transform()->SetRelativeRot(rot);
				CGameObject* Fir2 = SpawnPrefab(L"texture\\Prefab\\EXSFire2.prefab", EXSPos + Vec3(-100.f, 20.f, 0.f));
				Fir2->SetLifeSpan(0.1f);
				Fir2->Transform()->SetRelativeRot(rot);
				CGameObject* EXSbullet = SpawnPrefab(L"texture\\Prefab\\EXSBullet.prefab", EXSPos + Vec3(-100.f, 20.f, 0.f));
				EXSbullet->AddComponent(new EXSBullet);
				EXSBullet* pScript = EXSbullet->GetScript<EXSBullet>();
				pScript->SetDamage(Damage);
				pScript->SetBulletDir(false);
				
			}
			time = 0.f;
		}
	}
	else
	{//대포 발사
		if (0.4f<time )
		{
			rightcount++;
			if (rightcount !=6 &&rightcount !=7&& rightcount != 13&& rightcount != 14)
			{
				SpawnPrefab(L"texture\\Prefab\\EXSFire1.prefab", EXSPos + Vec3(120.f, 20.f, 0.f), 0.1f);
				SpawnPrefab(L"texture\\Prefab\\EXSFire2.prefab", EXSPos + Vec3(100.f, 20.f, 0.f), 0.1f);
				CGameObject* EXSbullet = SpawnPrefab(L"texture\\Prefab\\EXSBullet.prefab", EXSPos + Vec3(100.f, 20.f, 0.f));
				EXSbullet->AddComponent(new EXSBullet);
				EXSBullet* pScript = EXSbullet->GetScript<EXSBullet>();
				pScript->SetDamage(Damage);
				pScript->SetBulletDir(true);
				
			}
			time = 0.f;
		}
	}
	
	//깊이 판정
	vector<CGameObject*> vecObj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(0)->GetParentObject();
	for (size_t i = 0; i < vecObj.size(); i++)
	{
		if (L"tree" == vecObj[i]->GetName())
		{
			Vec3 treePos = vecObj[i]->Transform()->GetRelativePos();
			Vec3 treeScale = vecObj[i]->Transform()->GetRelativeScale();
			if (EXSPos.y <= treePos.y - 56.f)
			{
				EXSPos.z = treePos.z - 1.f;
				Transform()->SetRelativePos(EXSPos);
			}
			else if (EXSPos.y > treePos.y - 56.f)
			{
				EXSPos.z = treePos.z + 1.f;
				Transform()->SetRelativePos(EXSPos);
			}
		}
		else if (L"tree1" == vecObj[i]->GetName())
		{
			Vec3 treePos = vecObj[i]->Transform()->GetRelativePos();
			Vec3 treeScale = vecObj[i]->Transform()->GetRelativeScale();
			if (EXSPos.y <= treePos.y - treeScale.y / 2 - 56.f)
			{
				EXSPos.z = treePos.z - 1.f;
				Transform()->SetRelativePos(EXSPos);
			}
			else if (EXSPos.y > treePos.y - treeScale.y / 2 - 56.f)
			{
				EXSPos.z = treePos.z + 1.f;
				Transform()->SetRelativePos(EXSPos);
			}
		}
		else if (L"stone" == vecObj[i]->GetName())
		{
			Vec3 stonePos = vecObj[i]->Transform()->GetRelativePos();
			if (EXSPos.y <= stonePos.y - 56.f)
			{
				EXSPos.z = stonePos.z - 1.f;
				Transform()->SetRelativePos(EXSPos);
			}
			else if (EXSPos.y > stonePos.y - 56.f)
			{
				EXSPos.z = stonePos.z + 1.f;
				Transform()->SetRelativePos(EXSPos);
			}
		}
	}
	vector<CGameObject*> playerobj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
	if (L"player" == playerobj[0]->GetName())
	{
		Vec3 playerPos = playerobj[0]->Transform()->GetRelativePos();
		if (EXSPos.y <= playerPos.y - 56.f)
		{
			EXSPos.z = playerPos.z - 1.f;
			Transform()->SetRelativePos(EXSPos);
		}
		else if (EXSPos.y > playerPos.y - 56.f)
		{
			EXSPos.z = playerPos.z + 1.f;
			Transform()->SetRelativePos(EXSPos);
		}
	}
	vector<CGameObject*> breakObj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(8)->GetParentObject();
	for (size_t i = 0; i < breakObj.size(); i++)
	{
		if (L"h_obj1" == breakObj[i]->GetName())
		{
			Vec3 objPos = breakObj[i]->Transform()->GetRelativePos();
			if (EXSPos.y <= objPos.y)
			{
				objPos.z = EXSPos.z + 1.f;//뒤에 보이게
				breakObj[i]->Transform()->SetRelativePos(objPos);
			}
			else if (EXSPos.y > objPos.y)
			{
				objPos.z = EXSPos.z - 1.f;//앞에보이게
				breakObj[i]->Transform()->SetRelativePos(objPos);
			}
		}
	}
}

void CEXSScript::BeginOverlap(CCollider2D* _Other)
{

}

void CEXSScript::OnOverlap(CCollider2D* _Other)
{
}

void CEXSScript::EndOverlap(CCollider2D* _Other)
{
}
