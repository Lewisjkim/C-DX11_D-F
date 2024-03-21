#include "pch.h"
#include "CDRunnerScript.h"
#include "CPlayerScript.h"
#include "CGoblinScript.h"
#include "CGoblinThrowerScript.h"
#include "CTauScript.h"
#include "CBakalScript.h"

CDRunnerScript::CDRunnerScript() :
	CScript((UINT)SCRIPT_TYPE::DRUNNERSCRIPT),
	Speed(100.f),
	CastingTime(0.3f),
	CoolTime(2.1f),
	DurationTime(10.f),
	ExplodeRange(2.69f)
{
	b_dir = false;

	vector<CGameObject*> vecObj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
	for (size_t i = 0; i < vecObj.size(); i++)
	{
		b_dir = vecObj[i]->GetScript<CPlayerScript>()->GetDir();
	}
}

CDRunnerScript::~CDRunnerScript()
{
}

void CDRunnerScript::begin()
{
	CGameObject* DRunner = GetOwner();
	DRunner->Collider2D()->SetOffsetScale(Vec2(1.69f, 1.69f));
	GetOwner()->Animator2D()->LoadAnimation(L"anim\\DimensionCrack.anim");
	GetOwner()->Animator2D()->LoadAnimation(L"anim\\DimensionExplode.anim");
	GetOwner()->Animator2D()->LoadAnimation(L"anim\\DimensionHold.anim");
	GetOwner()->Animator2D()->LoadAnimation(L"anim\\DimensionStand.anim");
	//GetOwner()->Animator2D()->LoadAnimation(L"anim\\DimensionTarget.anim");
	//DimensionStand
	SetLifeSpan(DurationTime);
	isExplode = false;
	Ptr<CSound> s_DRStart = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunnerskill\\dimension_runner_tx_80.ogg");
	s_DRStart->Play(1, 0.3f, false);
}

void CDRunnerScript::tick()
{
	Ptr<CSound> s_DRexplode = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunnerskill\\dimension_runner_tx_80_shot_exp_02.ogg");
	Ptr<CSound> s_DRCrack = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunnerskill\\dimension_runner_tx_80_exp.ogg");
	
	Vec3 DRPos = Transform()->GetRelativePos();

	if (b_dir)
		DRPos.x += DT * Speed;
	else
	{
		Vec3 rot = Vec3(0.f, -180.f, 0.f);
		rot = (rot / 180.f) * XM_PI;
		GetOwner()->Transform()->SetRelativeRot(rot);
		DRPos.x -= DT * Speed;
	}

	//깊이 판정
	vector<CGameObject*> vecObj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(0)->GetParentObject();
	for (size_t i = 0; i < vecObj.size(); i++)
	{
		if (L"tree" == vecObj[i]->GetName())
		{
			Vec3 treePos = vecObj[i]->Transform()->GetRelativePos();
			Vec3 treeScale = vecObj[i]->Transform()->GetRelativeScale();
			if (DRPos.y <= treePos.y + 21.f)
			{
				DRPos.z = treePos.z - 1.f;
				Transform()->SetRelativePos(DRPos);
			}
			else if (DRPos.y > treePos.y + 21.f)
			{
				DRPos.z = treePos.z + 1.f;
				Transform()->SetRelativePos(DRPos);
			}
		}
		else if (L"tree1" == vecObj[i]->GetName())
		{
			Vec3 treePos = vecObj[i]->Transform()->GetRelativePos();
			Vec3 treeScale = vecObj[i]->Transform()->GetRelativeScale();
			if (DRPos.y <= treePos.y - treeScale.y / 2 + 21.f)
			{
				DRPos.z = treePos.z - 1.f;
				Transform()->SetRelativePos(DRPos);
			}
			else if (DRPos.y > treePos.y - treeScale.y / 2 + 21.f)
			{
				DRPos.z = treePos.z + 1.f;
				Transform()->SetRelativePos(DRPos);
			}
		}
		else if (L"stone" == vecObj[i]->GetName())
		{
			Vec3 stonePos = vecObj[i]->Transform()->GetRelativePos();
			if (DRPos.y <= stonePos.y + 21.f)
			{
				DRPos.z = stonePos.z - 1.f;
				Transform()->SetRelativePos(DRPos);
			}
			else if (DRPos.y > stonePos.y + 21.f)
			{
				DRPos.z = stonePos.z + 1.f;
				Transform()->SetRelativePos(DRPos);
			}
		}
	}
	vector<CGameObject*> playerobj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
	if (L"player" == playerobj[0]->GetName())
	{
		Vec3 playerPos = playerobj[0]->Transform()->GetRelativePos();
		if (DRPos.y <= playerPos.y + 21.f)
		{
			DRPos.z = playerPos.z - 1.f;
			Transform()->SetRelativePos(DRPos);
		}
		else if (DRPos.y > playerPos.y + 21.f)
		{
			DRPos.z = playerPos.z + 1.f;
			Transform()->SetRelativePos(DRPos);
		}
	}
	vector<CGameObject*> breakObj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(8)->GetParentObject();
	for (size_t i = 0; i < breakObj.size(); i++)
	{
		if (L"h_obj1" == breakObj[i]->GetName())
		{
			Vec3 objPos = breakObj[i]->Transform()->GetRelativePos();
			if (DRPos.y <= objPos.y)
			{
				objPos.z = DRPos.z + 1.f;//뒤에 보이게
				breakObj[i]->Transform()->SetRelativePos(objPos);
			}
			else if (DRPos.y > objPos.y)
			{
				objPos.z = DRPos.z - 1.f;//앞에보이게
				breakObj[i]->Transform()->SetRelativePos(objPos);
			}
		}
	}
	Transform()->SetRelativePos(DRPos);
	if (isExplode)
	{
		s_DRexplode->Play(1, 0.3f, false);
		s_DRCrack->Play(1, 0.3f, false);
		SpawnPrefab(L"texture\\Prefab\\Spark.prefab", DRPos, 1.2f);
		SpawnPrefab(L"texture\\Prefab\\DRunnerCrack.prefab", DRPos + Vec3(0.f, -50.f, +1.f), 0.5f);
		SpawnPrefab(L"texture\\Prefab\\DRunnerExplode.prefab", DRPos + Vec3(0.f, 0.f, -1.f), 0.8f);
		DestroyObject(this->GetOwner());
	}
}

void CDRunnerScript::BeginOverlap(CCollider2D* _Other)
{
}

void CDRunnerScript::OnOverlap(CCollider2D* _Other)
{
	Ptr<CSound> s_DRtarget = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunnerskill\\dimension_runner_tx_80_target.ogg");

	if (L"goblin" == _Other->GetOwner()->GetName())
	{
		Vec3 MonsterPos = _Other->GetOwner()->Transform()->GetRelativePos();
		Vec3 DRPos = GetOwner()->Transform()->GetRelativePos();
		Vec3 dir = MonsterPos - DRPos;
		dir = dir.Normalize();
		DRPos += dir * DT * Speed;
		GetOwner()->Transform()->SetRelativePos(DRPos);
		if (MonsterPos.x + 50.f >= DRPos.x || MonsterPos.x - 50.f < DRPos.x ||
			MonsterPos.y + 50.f > DRPos.y || MonsterPos.y - 50.f < DRPos.y)
		{
			count += DT;
			if (count > 0.2f)
				Animator2D()->Play(L"DimensionHold", true);
			if (0.3f < count && count < 0.6f)
			{
				s_DRtarget->Play(1, 0.3f, false);
				SpawnPrefab(L"texture\\Prefab\\DRunnerTarget.prefab", MonsterPos, 0.5f);
			}
			if (count > 1.f)
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
		Vec3 DRPos = GetOwner()->Transform()->GetRelativePos();
		Vec3 dir = MonsterPos - DRPos;
		dir = dir.Normalize();
		DRPos += dir * DT * Speed;
		GetOwner()->Transform()->SetRelativePos(DRPos);
		if (MonsterPos.x + 50.f >= DRPos.x || MonsterPos.x - 50.f < DRPos.x ||
			MonsterPos.y + 50.f > DRPos.y || MonsterPos.y - 50.f < DRPos.y)
		{
			count += DT;
			if (count > 0.2f)
				Animator2D()->Play(L"DimensionHold", true);
			if (0.3f < count && count < 0.6f)
			{
				s_DRtarget->Play(1, 0.3f, false);
				SpawnPrefab(L"texture\\Prefab\\DRunnerTarget.prefab", MonsterPos, 0.5f);
			}
			if (count > 1.f)
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
		Vec3 DRPos = GetOwner()->Transform()->GetRelativePos();
		Vec3 dir = MonsterPos - DRPos;
		dir = dir.Normalize();
		DRPos += dir * DT * Speed;
		GetOwner()->Transform()->SetRelativePos(DRPos);
		if (MonsterPos.x + 50.f >= DRPos.x || MonsterPos.x - 50.f < DRPos.x ||
			MonsterPos.y + 50.f > DRPos.y || MonsterPos.y - 50.f < DRPos.y)
		{
			count += DT;
			if (count > 0.2f)
				Animator2D()->Play(L"DimensionHold", true);
			if (0.3f < count && count < 0.6f)
			{
				s_DRtarget->Play(1, 0.3f, false);
				SpawnPrefab(L"texture\\Prefab\\DRunnerTarget.prefab", MonsterPos, 0.5f);
			}
			if (count > 1.f)
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
		Vec3 DRPos = GetOwner()->Transform()->GetRelativePos();
		Vec3 dir = MonsterPos - DRPos;
		dir = dir.Normalize();
		DRPos += dir * DT * Speed;
		GetOwner()->Transform()->SetRelativePos(DRPos);
		if (MonsterPos.x + 50.f >= DRPos.x || MonsterPos.x - 50.f < DRPos.x ||
			MonsterPos.y + 50.f > DRPos.y || MonsterPos.y - 50.f < DRPos.y)
		{
			count += DT;
			if (count > 0.2f)
				Animator2D()->Play(L"DimensionHold", true);
			if (0.3f < count && count < 0.6f)
			{
				s_DRtarget->Play(1, 0.3f, false);
				SpawnPrefab(L"texture\\Prefab\\DRunnerTarget.prefab", MonsterPos, 0.5f);
			}
			if (count > 1.f)
			{
				count = 0;
				CBakalScript* monsterscript = _Other->GetOwner()->GetScript<CBakalScript>();
				monsterscript->GetDamageFromPlayer(Damage);
				isExplode = true;
			}
		}
	}
}

void CDRunnerScript::EndOverlap(CCollider2D* _Other)
{
	if (3 == _Other->GetOwner()->GetLayerIndex())
	{
		Animator2D()->Play(L"DimensionStand", true);
	}
}
