#include "pch.h"
#include "CRX78Script.h"
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include "CGoblinScript.h"
#include "CGoblinThrowerScript.h"
#include "CTauScript.h"
#include "CBakalScript.h"
#include "CPlayerScript.h"

CRX78Script::CRX78Script() :
	CScript((UINT)SCRIPT_TYPE::RX78SCRIPT),
	Speed(50.f),
	CastingTime(0.3f),
	CoolTime(2.1f),
	DurationTime(7.f),
	ExplodeRange(1.69f)
{
	b_dir = false;
	
	vector<CGameObject*> vecObj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
	for (size_t i = 0; i < vecObj.size(); i++)
	{
		b_dir = vecObj[i]->GetScript<CPlayerScript>()->GetDir();
	}
}

CRX78Script::~CRX78Script()
{
}

void CRX78Script::begin()
{
	CGameObject* RX78 = GetOwner();
	RX78->Collider2D()->SetOffsetScale(Vec2(1.69f, 1.69f));
	GetOwner()->Animator2D()->LoadAnimation(L"anim\\BombEffect.anim");
	GetOwner()->Animator2D()->LoadAnimation(L"anim\\RX-78Jump.anim");
	GetOwner()->Animator2D()->LoadAnimation(L"anim\\RX-78Walk.anim");
	SetLifeSpan(DurationTime);
	isExplode = false;
}

void CRX78Script::tick()
{
	Ptr<CSound> s_RX78move = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunnerskill\\robot_move.ogg");
	Ptr<CSound> s_explosion = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunnerskill\\dimension_runner_tx_80_shot_exp_02.ogg");
	s_RX78move->Play(0, 0.2f, false);
	if (0 < Lv && Lv < 3)
	{
		MpUse = 13;
	}
	else if (2 < Lv && Lv < 5)
	{
		MpUse = 15;
	}
	else if (4 < Lv)
	{
		MpUse = 17;
	}
	Vec3 RX78Pos = GetOwner()->Transform()->GetRelativePos();
	if (b_dir)
		RX78Pos.x += DT * Speed;
	else
	{
		Vec3 rot = Vec3(0.f, -180.f, 0.f);
		rot = (rot / 180.f) * XM_PI;
		GetOwner()->Transform()->SetRelativeRot(rot);
		RX78Pos.x -= DT * Speed;
	}
	Vec3 RXPos = Transform()->GetRelativePos();
	Vec3 RXScale = Transform()->GetRelativeScale();

	if (isExplode)
	{
		SpawnPrefab(L"texture\\Prefab\\Spark.prefab", RXPos, 1.2f);
		SpawnPrefab(L"texture\\Prefab\\BombEffect.prefab", RXPos+Vec3(30.f,30.f,-1.f), 0.3f);
		SpawnPrefab(L"texture\\Prefab\\BombEffect2.prefab", RXPos+Vec3(-20.f,0.f,-1.f), 0.5f);
		s_explosion->Play(0, 0.2f, false);
		DestroyObject(this->GetOwner());
	}

	//깊이 판정
	vector<CGameObject*> vecObj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(0)->GetParentObject();
	for (size_t i = 0; i < vecObj.size(); i++)
	{
		if (L"tree" == vecObj[i]->GetName())
		{
			Vec3 treePos = vecObj[i]->Transform()->GetRelativePos();
			Vec3 treeScale = vecObj[i]->Transform()->GetRelativeScale();
			if (RX78Pos.y <= treePos.y - 42.f)
			{
				RX78Pos.z = treePos.z - 1.f;
				Transform()->SetRelativePos(RX78Pos);
			}
			else if (RX78Pos.y > treePos.y - 42.f)
			{
				RX78Pos.z = treePos.z + 1.f;
				Transform()->SetRelativePos(RX78Pos);
			}
		}
		else if (L"tree1" == vecObj[i]->GetName())
		{
			Vec3 treePos = vecObj[i]->Transform()->GetRelativePos();
			Vec3 treeScale = vecObj[i]->Transform()->GetRelativeScale();
			if (RX78Pos.y <= treePos.y - treeScale.y / 2 - 42.f)
			{
				RX78Pos.z = treePos.z - 1.f;
				Transform()->SetRelativePos(RX78Pos);
			}
			else if (RX78Pos.y > treePos.y - treeScale.y / 2 - 42.f)
			{
				RX78Pos.z = treePos.z + 1.f;
				Transform()->SetRelativePos(RX78Pos);
			}
		}
		else if (L"stone" == vecObj[i]->GetName())
		{
			Vec3 stonePos = vecObj[i]->Transform()->GetRelativePos();
			if (RX78Pos.y <= stonePos.y - 42.f)
			{
				RX78Pos.z = stonePos.z - 1.f;
				Transform()->SetRelativePos(RX78Pos);
			}
			else if (RX78Pos.y > stonePos.y - 42.f)
			{
				RX78Pos.z = stonePos.z + 1.f;
				Transform()->SetRelativePos(RX78Pos);
			}
		}
	}
	vector<CGameObject*> playerobj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
	if (L"player" == playerobj[0]->GetName())
	{
		Vec3 playerPos = playerobj[0]->Transform()->GetRelativePos();
		if (RX78Pos.y <= playerPos.y-42.f)
		{
			RX78Pos.z = playerPos.z - 1.f;
			Transform()->SetRelativePos(RX78Pos);
		}
		else if (RX78Pos.y > playerPos.y-42.f)
		{
			RX78Pos.z = playerPos.z + 1.f;
			Transform()->SetRelativePos(RX78Pos);
		}
	}
	vector<CGameObject*> breakObj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(8)->GetParentObject();
	for (size_t i = 0; i < breakObj.size(); i++)
	{
		if (L"h_obj1" == breakObj[i]->GetName())
		{
			Vec3 objPos = breakObj[i]->Transform()->GetRelativePos();
			if (RX78Pos.y <= objPos.y)
			{
				objPos.z = RX78Pos.z + 1.f;//뒤에 보이게
				breakObj[i]->Transform()->SetRelativePos(objPos);
			}
			else if (RX78Pos.y > objPos.y)
			{
				objPos.z = RX78Pos.z - 1.f;//앞에보이게
				breakObj[i]->Transform()->SetRelativePos(objPos);
			}
		}
	}
	GetOwner()->Transform()->SetRelativePos(RX78Pos);
}

void CRX78Script::BeginOverlap(CCollider2D* _Other)
{
	 
}

void CRX78Script::OnOverlap(CCollider2D* _Other)
{
	/*vector<CGameObject*> vecplayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
	CPlayerScript* plScript = vecplayer[0]->GetScript<CPlayerScript>();
	plScript->SetComboCount();*/
	if (L"goblin" == _Other->GetOwner()->GetName())
	{
		Vec3 MonsterPos = _Other->GetOwner()->Transform()->GetRelativePos();
		Vec3 RX78Pos = GetOwner()->Transform()->GetRelativePos();
		Vec3 dir = MonsterPos - RX78Pos;
		dir = dir.Normalize();
		RX78Pos += dir * DT * Speed;
		GetOwner()->Transform()->SetRelativePos(RX78Pos);
		if (MonsterPos.x + 10.f >= RX78Pos.x || MonsterPos.x - 10.f < RX78Pos.x ||
			MonsterPos.y + 10.f > RX78Pos.y || MonsterPos.y - 10.f < RX78Pos.y)
		{
			count += DT;
			if (count > 0.2f)
				Animator2D()->Play(L"RX-78Jump", true);
			if (count > 0.5f)
			{
				count = 0;
				CGoblinScript* monsterscript = _Other->GetOwner()->GetScript<CGoblinScript>();
				monsterscript->GetDamageFromPlayer(Damage);
				
				isExplode = true;
			}
		}
	}
	else if (L"goblinT" == _Other->GetOwner()->GetName())
	{
		Vec3 MonsterPos = _Other->GetOwner()->Transform()->GetRelativePos();
		Vec3 RX78Pos = GetOwner()->Transform()->GetRelativePos();
		Vec3 dir = MonsterPos - RX78Pos;
		dir = dir.Normalize();
		RX78Pos += dir * DT * Speed;
		GetOwner()->Transform()->SetRelativePos(RX78Pos);
		if (MonsterPos.x + 10.f >= RX78Pos.x || MonsterPos.x - 10.f < RX78Pos.x ||
			MonsterPos.y + 10.f > RX78Pos.y || MonsterPos.y - 10.f < RX78Pos.y)
		{
			count += DT;
			if (count > 0.2f)
				Animator2D()->Play(L"RX-78Jump", true);
			if (count > 0.5f)
			{
				
				count = 0;
				CGoblinThrowerScript* monsterscript = _Other->GetOwner()->GetScript<CGoblinThrowerScript>();

				monsterscript->GetDamageFromPlayer(Damage);
				isExplode = true;
			}

		}
	}
	else if (L"tau" == _Other->GetOwner()->GetName())
	{
		Vec3 MonsterPos = _Other->GetOwner()->Transform()->GetRelativePos();
		Vec3 RX78Pos = GetOwner()->Transform()->GetRelativePos();
		Vec3 dir = MonsterPos - RX78Pos;
		dir = dir.Normalize();
		RX78Pos += dir * DT * Speed;
		GetOwner()->Transform()->SetRelativePos(RX78Pos);
		if (MonsterPos.x + 10.f >= RX78Pos.x || MonsterPos.x - 10.f < RX78Pos.x ||
			MonsterPos.y + 10.f > RX78Pos.y || MonsterPos.y - 10.f < RX78Pos.y)
		{
			count += DT;
			if (count > 0.2f)
				Animator2D()->Play(L"RX-78Jump", true);
			if (count > 0.5f)
			{
				
				count = 0;
				CTauScript* monsterscript = _Other->GetOwner()->GetScript<CTauScript>();

				monsterscript->GetDamageFromPlayer(Damage);
				isExplode = true;
			}
		}
	}
	else if (L"Bakal" == _Other->GetOwner()->GetName())
	{
		Vec3 MonsterPos = _Other->GetOwner()->Transform()->GetRelativePos();
		Vec3 RX78Pos = GetOwner()->Transform()->GetRelativePos();
		Vec3 dir = MonsterPos - RX78Pos;
		dir = dir.Normalize();
		RX78Pos += dir * DT * Speed;
		GetOwner()->Transform()->SetRelativePos(RX78Pos);
		if (MonsterPos.x + 10.f >= RX78Pos.x || MonsterPos.x - 10.f < RX78Pos.x ||
			MonsterPos.y + 10.f > RX78Pos.y || MonsterPos.y - 10.f < RX78Pos.y)
		{
			count += DT;
			if (count < 0.2f)
				Animator2D()->Play(L"RX-78Jump", true);
			else if (0.2<count &&count < 0.5f)
			{
				//Animator2D()->Play(L"BombEffect", true);
				
				count = 0;
				CBakalScript* monsterscript = _Other->GetOwner()->GetScript<CBakalScript>();
				isExplode = true;
				monsterscript->GetDamageFromPlayer(Damage);
				
			}
		}
	}
}

void CRX78Script::EndOverlap(CCollider2D* _Other)
{
	if (3 == _Other->GetOwner()->GetLayerIndex())
	{
		Animator2D()->Play(L"RX-78Walk", true);
	}
}
