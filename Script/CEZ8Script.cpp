#include "pch.h"
#include "CEZ8Script.h"
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include "CGoblinScript.h"
#include "CPlayerScript.h"

CEZ8Script::CEZ8Script() :
	CScript((UINT)SCRIPT_TYPE::EZ8SCRIPT),
	Speed(100.f),
	CastingTime(0.3f),
	CoolTime(5.5f),
	DurationTime(8.f),
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

CEZ8Script::~CEZ8Script()
{
}

void CEZ8Script::begin()
{
	CGameObject* EZ8 = GetOwner();
	EZ8->Collider2D()->SetOffsetScale(Vec2(1.69f, 1.69f));
	GetOwner()->Animator2D()->LoadAnimation(L"anim\\BombEffect.anim");
	GetOwner()->Animator2D()->LoadAnimation(L"anim\\EZ-8Jump.anim");
	GetOwner()->Animator2D()->LoadAnimation(L"anim\\EZ-8Stay.anim");
	SetLifeSpan(DurationTime);
}

void CEZ8Script::tick()
{
	Ptr<CSound> s_EZ8 = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunnerskill\\robot_set_01.ogg");
	Ptr<CSound> s_EZ8count = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunnerskill\\robot_click_01.ogg");
	Ptr<CSound> s_explosion = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunnerskill\\dimension_runner_tx_80_shot_exp_02.ogg");

	s_EZ8->Play(0, 0.3f, false);
	s_EZ8count->Play(0, 0.3f, false);
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
	Vec3 EZ8Pos = GetOwner()->Transform()->GetRelativePos();
	if (b_dir ==false)
	{
		Vec3 rot = Vec3(0.f, -180.f, 0.f);
		rot = (rot / 180.f) * XM_PI;
		GetOwner()->Transform()->SetRelativeRot(rot);
	}
	//깊이 판정
	vector<CGameObject*> vecObj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(0)->GetParentObject();
	for (size_t i = 0; i < vecObj.size(); i++)
	{
		if (L"tree" == vecObj[i]->GetName())
		{
			Vec3 treePos = vecObj[i]->Transform()->GetRelativePos();
			Vec3 treeScale = vecObj[i]->Transform()->GetRelativeScale();
			if (EZ8Pos.y <= treePos.y - 56.f)
			{
				EZ8Pos.z = treePos.z - 1.f;
				Transform()->SetRelativePos(EZ8Pos);
			}
			else if (EZ8Pos.y > treePos.y - 56.f)
			{
				EZ8Pos.z = treePos.z + 1.f;
				Transform()->SetRelativePos(EZ8Pos);
			}
		}
		else if (L"tree1" == vecObj[i]->GetName())
		{
			Vec3 treePos = vecObj[i]->Transform()->GetRelativePos();
			Vec3 treeScale = vecObj[i]->Transform()->GetRelativeScale();
			if (EZ8Pos.y <= treePos.y - treeScale.y / 2 - 56.f)
			{
				EZ8Pos.z = treePos.z - 1.f;
				Transform()->SetRelativePos(EZ8Pos);
			}
			else if (EZ8Pos.y > treePos.y - treeScale.y / 2 - 56.f)
			{
				EZ8Pos.z = treePos.z + 1.f;
				Transform()->SetRelativePos(EZ8Pos);
			}
		}
		else if (L"stone" == vecObj[i]->GetName())
		{
			Vec3 stonePos = vecObj[i]->Transform()->GetRelativePos();
			if (EZ8Pos.y <= stonePos.y - 56.f)
			{
				EZ8Pos.z = stonePos.z - 1.f;
				Transform()->SetRelativePos(EZ8Pos);
			}
			else if (EZ8Pos.y > stonePos.y - 56.f)
			{
				EZ8Pos.z = stonePos.z + 1.f;
				Transform()->SetRelativePos(EZ8Pos);
			}
		}
	}
	vector<CGameObject*> playerobj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
	if (L"player" == playerobj[0]->GetName())
	{
		Vec3 playerPos = playerobj[0]->Transform()->GetRelativePos();
		if (EZ8Pos.y <= playerPos.y - 56.f)
		{
			EZ8Pos.z = playerPos.z - 1.f;
			Transform()->SetRelativePos(EZ8Pos);
		}
		else if (EZ8Pos.y > playerPos.y - 56.f)
		{
			EZ8Pos.z = playerPos.z + 1.f;
			Transform()->SetRelativePos(EZ8Pos);
		}
	}
	vector<CGameObject*> breakObj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(8)->GetParentObject();
	for (size_t i = 0; i < breakObj.size(); i++)
	{
		if (L"h_obj1" == breakObj[i]->GetName())
		{
			Vec3 objPos = breakObj[i]->Transform()->GetRelativePos();
			if (EZ8Pos.y <= objPos.y)
			{
				objPos.z = EZ8Pos.z + 1.f;//뒤에 보이게
				breakObj[i]->Transform()->SetRelativePos(objPos);
			}
			else if (EZ8Pos.y > objPos.y)
			{
				objPos.z = EZ8Pos.z - 1.f;//앞에보이게
				breakObj[i]->Transform()->SetRelativePos(objPos);
			}
		}
	}
	GetOwner()->Transform()->SetRelativePos(EZ8Pos);

	if (time > 4.f)
	{
		count += DT;
		if (count > 0.2f)
			Animator2D()->Play(L"EZ-8Jump", true);
		if (count > 0.5f)
		{
			SpawnPrefab(L"texture\\Prefab\\Spark.prefab", EZ8Pos, 1.2);
			SpawnPrefab(L"texture\\Prefab\\BombEffect.prefab", EZ8Pos + Vec3(30.f, 30.f, -1.f), 0.3);
			//SpawnPrefab(L"texture\\Prefab\\BombEffect.prefab", EZ8Pos + Vec3(-20.f, 0.f, -1.f), 0.5);
			isexplode = true;
			s_explosion->Play(0, 0.2f, false);
			DestroyObject(this->GetOwner());
		}
	}
}

void CEZ8Script::BeginOverlap(CCollider2D* _Other)
{
}

void CEZ8Script::OnOverlap(CCollider2D* _Other)
{
}

void CEZ8Script::EndOverlap(CCollider2D* _Other)
{
}
