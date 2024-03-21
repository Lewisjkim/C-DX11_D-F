#include "pch.h"
#include "CBakalScript.h"
#include "CPlayerScript.h"
#include "EXSBullet.h"
#include "CEZ8Script.h"
#include "CMissileScript.h"
#include "CRX78Script.h"
#include "CBakalAttackScript.h"
#include "CBakalStampScript.h"
#include "CDRunnerScript.h"

CBakalScript::CBakalScript() :
	CScript((UINT)SCRIPT_TYPE::BAKALSCRIPT),
	m_Speed(100.f),
	m_Hp(20000),
	m_MaxHp(20000),
	deadcount(0.f),
	m_exp(100)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
	AddScriptParam(SCRIPT_PARAM::INT, &m_Hp, "HP");
	AddScriptParam(SCRIPT_PARAM::INT, &m_MaxHp, "MaxHP");
	AddScriptParam(SCRIPT_PARAM::INT, &m_exp, "Exp Amount");
	
}

CBakalScript::~CBakalScript()
{
}

void CBakalScript::begin()
{
	MeshRender()->GetDynamicMaterial();
	Animator2D()->LoadAnimation(L"anim\\BakalLose.anim");
	Animator2D()->LoadAnimation(L"anim\\BakalStamp.anim"); 
	Animator2D()->LoadAnimation(L"anim\\BakalWalk.anim"); 
	Animator2D()->LoadAnimation(L"anim\\BakalAttack.anim");
	iswall = false;
	minus = 1;
	timer = 0.f;
	movetype = false;
	isRight = false;
	readytoAttack = false;
	isHit = false;
	isKnockback = false;
	isKneeKick = false;
	kneekickTime = 0.f;
	attackTerm = 0.f;
	isoverlap = false;
	rest = false;
	isattackRight = false;
	isattackLeft = false;
	isDead = false;
	Ptr<CSound> s_bakalstart = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\monster\\bakal_human_meet_01.ogg");
	s_bakalstart->Play(1, 0.3f, false);
}

void CBakalScript::tick()
{
	random = rand();
	movetype = random <= 12000 ? true : false;
	randomItem = random <= 18000 ? true : false;
	Ptr<CSound> s_bakallaugh = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\monster\\bakal_human_skill_03_2.ogg");
	Ptr<CSound> s_bakaldie = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\monster\\basilisk_die.ogg");
	
	BakalPos = Transform()->GetRelativePos();
	timer += DT;
	randomTime += DT;

	if (iswall)
	{
		m_time += DT;
		minus *= -1;
		timer = 0.f;
	}

	//벽 충돌로 후 변경된 속도값 초기화
	if (m_time > 3.5f)
	{
		m_time = 0;
		m_Speed *= -1;
	}

	//플레이어 방향 쳐다보기
	vector<CGameObject*> vecPlayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
	Vec3 playerPos = vecPlayer[0]->Transform()->GetRelativePos();

	playerPos.x = playerPos.x - 20.f;
	playerPos.y = playerPos.y - 20.f;
	isRight = BakalPos.x < playerPos.x ? true : false;
	Vec3 dir = Vec3(0.f, 0.f, 0.f);
	if (isRight)
	{
		Vec3(0.f, 0.f, 0.f);
		dir = (dir / 180.f) * XM_PI;
		Transform()->SetRelativeRot(dir);
	}
	else
	{
		dir = Vec3(0.f, -180.f, 0.f);
		dir = (dir / 180.f) * XM_PI;
		Transform()->SetRelativeRot(dir);
	}
	
	if (isKneeKick)
	{
		kneekickTime += DT;
		Vec3 KneeKickpos;
		vector<CGameObject*> vecPlayerAttack = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(4)->GetParentObject();
		for (size_t i = 0; i < vecPlayerAttack.size(); i++)
		{
			if (vecPlayerAttack[i]->GetName() == L"KneeKick")
			{
				Vec3 KneeKickpos = vecPlayerAttack[i]->Transform()->GetRelativePos();
			}
		}
		Vec3 dirVector = (BakalPos - KneeKickpos).Normalize();//킥->몬스터로의 방향 벡터
		if (kneekickTime < 0.3f)
		{
			BakalPos.x += dirVector.x * DT * 10;
			BakalPos.y += DT * 300;
		}
		else if (kneekickTime > 0.3f && kneekickTime < 0.6f)
		{
			BakalPos.x += dirVector.x * DT * 10;
			BakalPos.y -= DT * 150;
		}
		else if (kneekickTime > 0.6f && kneekickTime < 0.9f)
		{
			BakalPos.x += dirVector.x * DT * 10;
			BakalPos.y -= DT * 150;

		}
		else if (kneekickTime > 0.9f && kneekickTime < 1.2f)
		{
			/*BakalPos.x += dirVector.x * DT * 10;
			BakalPos.y -= DT * 50;*/
			kneekickTime = 0.f;
			isKneeKick = false;
			Animator2D()->Play(L"BakalWalk", true);
		}

		Transform()->SetRelativePos(BakalPos);

	}

	Vec3 dirVector = (playerPos+(Vec3(0.f,50.f,0.f)) - BakalPos).Normalize();
	Vec3 betweenLength = Vec3(0.f, 0.f, 0.f);
	betweenLength.x = abs(playerPos.x - BakalPos.x);
	betweenLength.y = abs(playerPos.y - BakalPos.y);
	if (iswall == false && isKneeKick == false && 4.f < betweenLength.x 
		&& betweenLength.x <= 200.f && 4.f < betweenLength.y && betweenLength.y <= 200.f)
	{
		readytoAttack = true;

		if (!isoverlap)//충돌하지 않았으면
		{
			BakalPos += dirVector * DT * m_Speed*3; // 플레이어 방향으로 이동
			Animator2D()->Play(L"BakalWalk", true);
			s_bakallaugh->Play(1, 0.3f, false);
		}
	}
	else if (isKneeKick == false && betweenLength.x <= 4.f && betweenLength.y <= 4.f)
	{
		readytoAttack = true;
	}
	else if (betweenLength.x > 200.f || betweenLength.y > 200.f)
	{
		Animator2D()->Play(L"BakalIdle", true);
		readytoAttack = false;
		
	}
	
	//공격시 앞으로 조금씩 이동
	if (isattackRight)
	{
		BakalPos.x = BakalPos.x + 100 * DT;
		isattackRight = false;
	}
	if (isattackLeft)
	{
		BakalPos.x = BakalPos.x + -100 * DT;
		isattackLeft = false;
	}
	
	//실시간 깊이 판정
	if (66.f < BakalPos.z && BakalPos.z < 510.f)
	{
		vector<CGameObject*> vecObj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(0)->GetParentObject();
		for (size_t i = 0; i < vecObj.size(); i++)
		{
			if (L"tree" == vecObj[i]->GetName())
			{
				Vec3 treePos = vecObj[i]->Transform()->GetRelativePos();
				Vec3 treeScale = vecObj[i]->Transform()->GetRelativeScale();
				if (BakalPos.y <= treePos.y)
				{
					BakalPos.z = treePos.z - 1.f;
					Transform()->SetRelativePos(BakalPos);
				}
				else if (BakalPos.y > treePos.y)
				{
					BakalPos.z = treePos.z + 1.f;
					Transform()->SetRelativePos(BakalPos);
				}
			}
			else if (L"tree1" == vecObj[i]->GetName())
			{
				Vec3 treePos = vecObj[i]->Transform()->GetRelativePos();
				Vec3 treeScale = vecObj[i]->Transform()->GetRelativeScale();
				if (BakalPos.y <= treePos.y - treeScale.y / 2)
				{
					BakalPos.z = treePos.z - 1.f;
					Transform()->SetRelativePos(BakalPos);
				}
				else if (BakalPos.y > treePos.y - treeScale.y / 2)
				{
					BakalPos.z = treePos.z + 1.f;
					Transform()->SetRelativePos(BakalPos);
				}
			}
			else if (L"stone" == vecObj[i]->GetName())
			{
				Vec3 stonePos = vecObj[i]->Transform()->GetRelativePos();
				if (BakalPos.y <= stonePos.y)
				{
					BakalPos.z = stonePos.z - 1.f;
					Transform()->SetRelativePos(BakalPos);
				}
				else if (BakalPos.y > stonePos.y)
				{
					BakalPos.z = stonePos.z + 1.f;
					Transform()->SetRelativePos(BakalPos);
				}
			}
		}
		vector<CGameObject*> playerobj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
		if (L"player" == playerobj[0]->GetName())
		{
			Vec3 playerPos = playerobj[0]->Transform()->GetRelativePos();
			if (BakalPos.y <= playerPos.y)
			{
				BakalPos.z = playerPos.z - 1.f;
				Transform()->SetRelativePos(BakalPos);
			}
			else if (BakalPos.y > playerPos.y)
			{
				BakalPos.z = playerPos.z + 1.f;
				Transform()->SetRelativePos(BakalPos);
			}
		}
		vector<CGameObject*> monobj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(3)->GetParentObject();
		for (size_t i = 0; i < monobj.size(); i++)
		{
			if (L"goblin" == monobj[i]->GetName())
			{
				Vec3 monPos = monobj[i]->Transform()->GetRelativePos();
				if (BakalPos.y <= monPos.y)
				{
					BakalPos.z = monPos.z - 1.f;
					Transform()->SetRelativePos(BakalPos);
				}
				else if (BakalPos.y > monPos.y)
				{
					BakalPos.z = monPos.z + 1.f;
					Transform()->SetRelativePos(BakalPos);
				}
			}
			else if (L"goblinT" == monobj[i]->GetName())
			{
				Vec3 monPos = monobj[i]->Transform()->GetRelativePos();
				if (BakalPos.y <= monPos.y)
				{
					BakalPos.z = monPos.z - 1.f;
					Transform()->SetRelativePos(BakalPos);
				}
				else if (BakalPos.y > monPos.y)
				{
					BakalPos.z = monPos.z + 1.f;
					Transform()->SetRelativePos(BakalPos);
				}
			}
			else if (L"tau" == monobj[i]->GetName())
			{
				Vec3 monPos = monobj[i]->Transform()->GetRelativePos();
				if (BakalPos.y <= monPos.y)
				{
					BakalPos.z = monPos.z - 1.f;
					Transform()->SetRelativePos(BakalPos);
				}
				else if (BakalPos.y > monPos.y)
				{
					BakalPos.z = monPos.z + 1.f;
					Transform()->SetRelativePos(BakalPos);
				}
			}
		}
		vector<CGameObject*> skillobj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(4)->GetParentObject();
		for (size_t i = 0; i < skillobj.size(); i++)
		{
			if (L"RX78" == skillobj[i]->GetName())
			{
				Vec3 skillPos = skillobj[i]->Transform()->GetRelativePos();
				if (BakalPos.y <= skillPos.y)
				{
					BakalPos.z = skillPos.z - 1.f;
					Transform()->SetRelativePos(BakalPos);
				}
				else if (BakalPos.y > skillPos.y)
				{
					BakalPos.z = skillPos.z + 1.f;
					Transform()->SetRelativePos(BakalPos);
				}
			}
			else if (L"EXS" == skillobj[i]->GetName())
			{
				Vec3 skillPos = skillobj[i]->Transform()->GetRelativePos();
				if (BakalPos.y <= skillPos.y)
				{
					BakalPos.z = skillPos.z - 1.f;
					Transform()->SetRelativePos(BakalPos);
				}
				else if (BakalPos.y > skillPos.y)
				{
					BakalPos.z = skillPos.z + 1.f;
					Transform()->SetRelativePos(BakalPos);
				}
			}
			else if (L"DRunner" == skillobj[i]->GetName())
			{
				Vec3 skillPos = skillobj[i]->Transform()->GetRelativePos();
				if (BakalPos.y <= skillPos.y)
				{
					BakalPos.z = skillPos.z - 1.f;
					Transform()->SetRelativePos(BakalPos);
				}
				else if (BakalPos.y > skillPos.y)
				{
					BakalPos.z = skillPos.z + 1.f;
					Transform()->SetRelativePos(BakalPos);
				}
			}
		}
	}
	
	
	// 이동 패턴
	if (readytoAttack==false && !isHit && isKneeKick == false&&isoverlap==false)
	{
		if (randomTime > 1.f)
		{
			m_Speed = rand();
			m_Speed = m_Speed >= 18000 ? 70.f : 50.f;
			monsterType = rand();
			movetype = monsterType <= 18000 ? true : false;
			randomTime = 0;
		}
		if (movetype)
		{
			if (timer < 1.f)
			{
				BakalPos.x += DT * m_Speed * 2 * minus;
				//BakalPos.y += DT * m_Speed * minus;
				Animator2D()->Play(L"BakalWalk", true);
			}
			else if (1.f < timer && timer < 3.f)
			{
				Animator2D()->Play(L"BakalIdle", true);
			}
			else if (timer < 3.f && timer < 4.f)
			{
				BakalPos.x -= DT * m_Speed * 2 * minus;
				//BakalPos.y += DT * m_Speed * minus;
				timer = 0.f;
				Animator2D()->Play(L"BakalWalk", true);
			}
		}
		else
		{
			if (timer < 1.f)
			{
				BakalPos.x += DT * m_Speed * 2 * minus;
				BakalPos.y += DT * m_Speed * minus;
				Animator2D()->Play(L"BakalWalk", true);
			}
			else if (1.f < timer && timer < 3.f)
			{
				Animator2D()->Play(L"BakalIdle", true);
			}
			else if (timer < 3.f && timer < 4.f)
			{
				BakalPos.x += DT * m_Speed * 2 * minus;
				BakalPos.y -= DT * m_Speed * minus;
				timer = 0.f;
				Animator2D()->Play(L"BakalWalk", true);
			}
			
		}
	}
	if(!isDead)
		Transform()->SetRelativePos(BakalPos);
	//hp<0 시 Set Dead
	if (m_Hp <= 0)
	{
		m_Hp = 0;
		deadcount += DT;
		
		//isDead = false;
		
		if (!isDead)
		{
			s_bakaldie->Play(1, 0.3f, false);
			SpawnPrefab(L"texture\\Prefab\\Coin.prefab", BakalPos - Vec3(0.f, 50.f, 0.f));
			if (randomItem)
				SpawnPrefab(L"texture\\Prefab\\HPPotion.prefab", BakalPos - Vec3(0.f, 50.f, 0.f));
			else
				SpawnPrefab(L"texture\\Prefab\\MPPotion.prefab", BakalPos - Vec3(0.f, 50.f, 0.f));
			vector<CGameObject*> maincamobj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(10)->GetParentObject();
			SpawnPrefab(L"texture\\Prefab\\Fade.prefab",Vec3(0.f,0.f,5.f));
			SpawnPrefab(L"texture\\Prefab\\GoBackToVillage.prefab", Vec3(0.f, 0.f, 0.f));
			vector<CGameObject*> playerobj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
			for (size_t i = 0; i < playerobj.size(); i++)
			{
				if (playerobj[i]->GetScript<CPlayerScript>())
				{
					CPlayerScript* script = playerobj[i]->GetScript<CPlayerScript>();
					script->GetExpFromMonster(m_exp);
				}
			}
		}
		isDead = true;

		Animator2D()->Play(L"BakalLose", false);
		if (deadcount > 1.f)
		{
			DestroyObject(this->GetOwner());
		}
			
	}
}

void CBakalScript::BeginOverlap(CCollider2D* _Other)
{
	Ptr<CSound> s_bakalhit = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\monster\\bakal_human_dmg_01.ogg");
	vector<CGameObject*> vecplayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
	CPlayerScript* plScript = vecplayer[0]->GetScript<CPlayerScript>();
	
	if (L"bullet" == _Other->GetOwner()->GetName())
	{
		plScript->SetTarget(GetOwner()->GetID());
		s_bakalhit->Play(1, 0.3f, false);
	}
	//대포알 충돌시
	else if (L"EXSRB" == _Other->GetOwner()->GetName())
	{
		plScript->SetTarget(GetOwner()->GetID());
		s_bakalhit->Play(1, 0.3f, false);
		EXSBullet* pScript = _Other->GetOwner()->GetScript<EXSBullet>();
		int EXSDamage = pScript->GetDamage();
		//Animator2D()->Play(L"goblin_hit", true);

		int count = GetDigitCount(EXSDamage);
		ShowDamage(EXSDamage, count, GetOwner());
		m_Hp -= EXSDamage;
		plScript->SetComboCount();
	}
	//밀어차기
	else if (L"FastKnee" == _Other->GetOwner()->GetName())
	{
		plScript->SetTarget(GetOwner()->GetID());
		s_bakalhit->Play(1, 0.3f, false);
		//넉백
		Vec3 bulletPos = _Other->GetOwner()->Transform()->GetRelativePos();
		Vec3 dirVector = (BakalPos - bulletPos).Normalize();
		int kickdistance = 20;
		for (size_t i = 0; i < 4; i++)
		{
			BakalPos.x += dirVector.x * kickdistance;
			Transform()->SetRelativePos(BakalPos);
			kickdistance - 3;
		}

		vector<CGameObject*> playerobj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
		for (size_t i = 0; i < playerobj.size(); i++)
		{
			if (playerobj[i]->GetScript<CPlayerScript>())
			{
				CPlayerScript* script = playerobj[i]->GetScript<CPlayerScript>();
				int FastKneeDamage = script->GetPAttack();
				int playerlevel = script->GetLevel();
				if (playerlevel == 1)
					FastKneeDamage *= 26;
				else if (1 < playerlevel && playerlevel < 4)
					FastKneeDamage *= 29;
				else if (5 <= playerlevel)
					FastKneeDamage *= 32;
				Animator2D()->Play(L"BakalLose", true);

				int count = GetDigitCount(FastKneeDamage);
				ShowDamage(FastKneeDamage, count, GetOwner());
				m_Hp -= FastKneeDamage;
				plScript->SetComboCount();
			}
		}
	}
	else if (L"KneeKick" == _Other->GetOwner()->GetName())
	{
		plScript->SetTarget(GetOwner()->GetID());
		s_bakalhit->Play(1, 0.3f, false);
		isKneeKick = true;

		//데미지
		vector<CGameObject*> playerobj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
		for (size_t i = 0; i < playerobj.size(); i++)
		{
			if (playerobj[i]->GetScript<CPlayerScript>())
			{
				CPlayerScript* script = playerobj[i]->GetScript<CPlayerScript>();
				int KneeKickDamage = script->GetPAttack();
				int playerlevel = script->GetLevel();
				if (playerlevel == 1)
					KneeKickDamage *= 11;
				else if (1 < playerlevel && playerlevel < 4)
					KneeKickDamage *= 12;
				else if (5 <= playerlevel)
					KneeKickDamage *= 13;
				Animator2D()->Play(L"BakalLose", true);

				int count = GetDigitCount(KneeKickDamage);
				ShowDamage(KneeKickDamage, count, GetOwner());
				m_Hp -= KneeKickDamage;
				plScript->SetComboCount();
			}
		}
	}
	else if (L"Windmil" == _Other->GetOwner()->GetName())
	{
		plScript->SetTarget(GetOwner()->GetID());
		s_bakalhit->Play(1, 0.3f, false);
		vector<CGameObject*> playerobj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
		for (size_t i = 0; i < playerobj.size(); i++)
		{
			if (playerobj[i]->GetScript<CPlayerScript>())
			{
				CPlayerScript* script = playerobj[i]->GetScript<CPlayerScript>();
				int WindmilDamage = script->GetPAttack();
				int playerlevel = script->GetLevel();
				if (playerlevel == 1)
					WindmilDamage *= 11.25f;
				else if (1 < playerlevel && playerlevel < 4)
					WindmilDamage *= 12.29f;
				else if (5 <= playerlevel)
					WindmilDamage *= 13.54f;
				//Animator2D()->Play(L"goblin_hit", true);

				int count = GetDigitCount(WindmilDamage);
				for (size_t i = 0; i < 2; i++)
				{
					ShowDamage(WindmilDamage, count, GetOwner());
					m_Hp -= WindmilDamage;
					plScript->SetComboCount();
				}
			}
		}
	}
	else if (9 == _Other->GetOwner()->GetLayerIndex())//벽
	{
		iswall = true;
	}
	else if (L"player" == _Other->GetOwner()->GetName())
	{
		isoverlap = true;
	}
}

void CBakalScript::OnOverlap(CCollider2D* _Other)
{
	Ptr<CSound> s_bakalhit = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\monster\\bakal_human_dmg_01.ogg");
	Ptr<CSound> s_bakalexplode = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\monster\\bakal_human_attack.ogg");
	Ptr<CSound> s_bakalexplode2 = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\monster\\bakal_human_fireball_02.ogg");
	Ptr<CSound> s_bakalClawattack = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\monster\\bakal_human_attack_wind_02.ogg");
	Ptr<CSound> s_bakalClawattack2 = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\monster\\bakal_human_hit_01.ogg");

	Ptr<CSound> s_bakalstamp = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\monster\\bakal_human_earth_attack.ogg");
	
	vector<CGameObject*> vecplayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
	CPlayerScript* plScript = vecplayer[0]->GetScript<CPlayerScript>();
	
	if (L"EZ8" == _Other->GetOwner()->GetName())
	{
		plScript->SetTarget(GetOwner()->GetID());
		s_bakalhit->Play(1, 0.3f, false);
		CEZ8Script* pScript = _Other->GetOwner()->GetScript<CEZ8Script>();
		int EZ8Damage = pScript->GetDamage();
		if (pScript->IsExplode())
		{
			int count = GetDigitCount(EZ8Damage);
			ShowDamage(EZ8Damage, count, GetOwner());
			m_Hp -= EZ8Damage;
			plScript->SetComboCount();
		}
	}
	else if (L"player" == _Other->GetOwner()->GetName())
	{
		int hitrandom = rand();
		bool attacktype = hitrandom >= 16000 ? true : false;
		//bool attacktype = false;
		isoverlap = true;
		attackTerm += DT;
		//내력찍기 패턴1
		if (attacktype&& rest == false)
		{
			if (1.f < attackTerm && attackTerm < 1.2f)
			{
				s_bakalstamp->Play(1, 0.3f, false);
				
				Animator2D()->Play(L"BakalStamp", false);//Stamp
				Animator2D()->GetCurAnim()->Reset();
			}
			else if (2.f < attackTerm && attackTerm < 2.6f)
			{
				if (isRight)
				{
					pDust = SpawnPrefab(L"texture\\Prefab\\Dust.prefab", BakalPos);//Dust
					pDust->SetName(L"BakalStampDust");
					pDust->SetLifeSpan(0.5f);
					pMeteo = SpawnPrefab(L"texture\\Prefab\\Meteo.prefab", BakalPos);//Meteo
					pMeteo->SetName(L"BakalMeteo");
					pMeteo->SetLifeSpan(0.5f);
					s_bakalexplode->Play(1, 0.3f, false);
					pMeteo2 = SpawnPrefab(L"texture\\Prefab\\Meteo.prefab", BakalPos + Vec3(-100.f,0.f,0.f));//Meteo
					pMeteo2->SetName(L"BakalMeteo2");
					pMeteo2->SetLifeSpan(0.5f);
					s_bakalexplode2->Play(1, 0.3f, false);
					pMeteo3 = SpawnPrefab(L"texture\\Prefab\\Meteo.prefab", BakalPos + Vec3(100.f, 0.f, 0.f));//Meteo
					pMeteo3->SetName(L"BakalMeteo3");
					pMeteo3->SetLifeSpan(0.5f);
				}
				else
				{
					pDust = SpawnPrefab(L"texture\\Prefab\\Dust.prefab", BakalPos);//Dust
					pDust->SetName(L"BakalStampDust");
					pDust->SetLifeSpan(0.5f);
					pMeteo = SpawnPrefab(L"texture\\Prefab\\Meteo.prefab", BakalPos);//Meteo
					pMeteo->SetName(L"BakalMeteo");
					pMeteo->SetLifeSpan(0.5f);
					pMeteo2 = SpawnPrefab(L"texture\\Prefab\\Meteo.prefab", BakalPos + Vec3(-100.f, 100.f, 0.f));//Meteo
					pMeteo2->SetName(L"BakalMeteo2");
					pMeteo2->SetLifeSpan(0.5f);
					pMeteo3 = SpawnPrefab(L"texture\\Prefab\\Meteo.prefab", BakalPos + Vec3(100.f, 100.f, 0.f));//Meteo
					pMeteo3->SetName(L"BakalMeteo3");
					pMeteo3->SetLifeSpan(0.5f);
				}
				rest = true;
			}
		}
		//후려치기 패턴2
		else if(attacktype== false && rest == false)
		{
			if (1.f < attackTerm)
			{
				s_bakalClawattack->Play(1, 0.3f, false);
				s_bakalClawattack2->Play(1, 0.3f, false);
				Animator2D()->Play(L"BakalAttack", false);
				Animator2D()->GetCurAnim()->Reset();
				if (isRight)
				{
					isattackRight = true;
					rightattack = SpawnPrefab(L"texture\\Prefab\\RightBakalAttackEffect.prefab", BakalPos);
					rightattack->SetName(L"BakalAttack");
					rightattack->AddComponent(new CBakalAttackScript);
				}
				else
				{
					isattackLeft = true;
					leftattack = SpawnPrefab(L"texture\\Prefab\\RightBakalAttackEffect.prefab", BakalPos);
					leftattack->SetName(L"BakalAttack");
					leftattack->AddComponent(new CBakalAttackScript);
				}
				rest = true;
			}
			
		}

		if (rest)
		{
			if (2.f < attackTerm&&attackTerm<3.f)
			{
				isattackRight = false;
				isattackLeft = false;
			}
			else if (3.3f < attackTerm&& attackTerm<4.f)
			{
				Animator2D()->Play(L"BakalIdle", true);
			}
			else if (4.f < attackTerm)
			{
				rest = false;
				attackTerm = 0.f;
			}
		}
	}
}

void CBakalScript::EndOverlap(CCollider2D* _Other)
{
	Ptr<CSound> s_bakalhit = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\monster\\bakal_human_dmg_01.ogg");
	vector<CGameObject*> vecplayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
	CPlayerScript* plScript = vecplayer[0]->GetScript<CPlayerScript>();

	if (L"bullet" == _Other->GetOwner()->GetName())
	{
		CMissileScript* pScript = _Other->GetOwner()->GetScript<CMissileScript>();
		int bulletDamage = pScript->GetBulletDamage();

		int count = GetDigitCount(bulletDamage);
		ShowDamage(bulletDamage, count, GetOwner());
		m_Hp -= bulletDamage;
		plScript->SetComboCount();
		isHit = true;
	}
	else if (L"RX78" == _Other->GetOwner()->GetName())
	{
		plScript->SetTarget(GetOwner()->GetID());
		s_bakalhit->Play(1, 0.3f, false);
		//범위내 모든 고블린들을  동시에 데미주려고
		CRX78Script* pScript = _Other->GetOwner()->GetScript<CRX78Script>();
		int RX78Damage = pScript->GetDamage();

		//10의 (자리수 제곱)
		int count = GetDigitCount(RX78Damage);
		ShowDamage(RX78Damage, count, GetOwner());
		plScript->SetComboCount();
		m_Hp -= RX78Damage;
	}
	else if (L"EXSRB" == _Other->GetOwner()->GetName())
	{
		if (isKnockback)
		{
			isKnockback = false;
		}
		Animator2D()->Play(L"BakalIdle", true);
	}
	else if (L"EZ8" == _Other->GetOwner()->GetName())
	{
		s_bakalhit->Play(1, 0.3f, false);
	}
	else if (L"FastKnee" == _Other->GetOwner()->GetName())
	{
		Animator2D()->Play(L"BakalIdle", true);
	}
	else if (L"Windmil" == _Other->GetOwner()->GetName())
	{
		Animator2D()->Play(L"BakalIdle", true);
	}
	else if (L"player" == _Other->GetOwner()->GetName())
	{
		//attackTerm = 0.f;
		isoverlap = false;
		timer = 0.f;
		Animator2D()->Play(L"BakalIdle", true);
	}
	else if (L"DRunner" == _Other->GetOwner()->GetName())
	{
		plScript->SetTarget(GetOwner()->GetID());
		//범위내 모든 고블린들을  동시에 데미주려고
		CDRunnerScript* pScript = _Other->GetOwner()->GetScript<CDRunnerScript>();
		int RX78Damage = pScript->GetDamage();

		//10의 (자리수 제곱)
		int count = GetDigitCount(RX78Damage);
		ShowDamage(RX78Damage, count, GetOwner());
		plScript->SetComboCount();
		m_Hp -= RX78Damage;
	}
	else if (9 == _Other->GetOwner()->GetLayerIndex())//벽
	{
		iswall = false;
	}
}
