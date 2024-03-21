#include "pch.h"
#include "CTauScript.h"
#include "CMissileScript.h"
#include "CRX78Script.h"
#include "CEZ8Script.h"
#include "EXSBullet.h"
#include "CDRunnerScript.h"

CTauScript::CTauScript()		:
	CScript((UINT)SCRIPT_TYPE::TAUSCRIPT),
	m_Damage(150),
	m_Speed(100.f),
	m_Hp(10000),
	m_MaxHp(10000),
	deadcount(0.f),
	m_exp(60)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
	AddScriptParam(SCRIPT_PARAM::INT, &m_Hp, "HP");
	AddScriptParam(SCRIPT_PARAM::INT, &m_MaxHp, "MaxHP");
	AddScriptParam(SCRIPT_PARAM::INT, &m_exp, "Exp Amount");
	
}

CTauScript::~CTauScript()
{
}

void CTauScript::begin()
{
	MeshRender()->GetDynamicMaterial();
	Animator2D()->LoadAnimation(L"anim\\TauAttack.anim");
	Animator2D()->LoadAnimation(L"anim\\TauCharge.anim"); 
	Animator2D()->LoadAnimation(L"anim\\TauHit.anim");
	Animator2D()->LoadAnimation(L"anim\\TauKDown.anim");
	Animator2D()->LoadAnimation(L"anim\\TauSkill.anim");
	iswall = false;
	minus = 1;
	timer = 0.f;
	movetype = false;
	isLeft = false;
	readytoAttack = false;
	isHit = false;
	isKnockback = false;
	isKneeKick = false;
	kneekickTime = 0.f;
	attackTerm = 0.f;
	setdir = false;
	rest = 0.f;
	isDead = false;
	Ptr<CSound> s_tauroar = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\monster\\tau_roar.ogg");
	s_tauroar->Play(1, 0.3f, false);
}

void CTauScript::tick()
{
	random = rand();
	movetype = random <= 16, 383 ? true : false;
	randomItem = random <= 16, 383 ? true : false;

	Ptr<CSound> s_tauattack = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\monster\\tau_eq_cast.ogg");
	Ptr<CSound> s_taudie = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\monster\\tau_die.ogg");
	
	TauPos = Transform()->GetRelativePos();
	timer += DT;
	randomTime += DT;
	chargetime += DT;
	//�� �浹 �� �ӵ��� ����(������ȯ)
	if (iswall)
	{
		m_time += DT;
		minus *= -1;
		timer = 0.f;
	}

	//�� �浹 �� ����� �ӵ��� �ʱ�ȭ
	if (m_time > 3.5f)
	{
		m_time = 0;
		m_Speed *= -1;
	}

	//�÷��̾� ���� �Ĵٺ���
	vector<CGameObject*> vecPlayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
	Vec3 playerPos = vecPlayer[0]->Transform()->GetRelativePos();

	playerPos.x = playerPos.x - 20.f;
	playerPos.y = playerPos.y - 20.f;
	isLeft = TauPos.x < playerPos.x ? true : false;
	Vec3 dir = Vec3(0.f, 0.f, 0.f);
	if (isLeft)
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

	

	//����
	Vec3 betweenLength = Vec3(0.f, 0.f, 0.f);
	betweenLength.x = abs(playerPos.x - TauPos.x);
	betweenLength.y = abs(playerPos.y - TauPos.y);
	Vec3 dirVector;
	if (iswall == false && isKneeKick == false && 3.f<betweenLength.x &&  betweenLength.x <= 200.f && 3.f < betweenLength.y && betweenLength.y <= 200.f)
	{
		readytoRush = true;
		//200���� �Ÿ��� ������  ����(�׵��� �Ϲ� ���� �ȵ�)
		rest += DT;
		if (rest < 1.f)
		{
			dirVector = (playerPos + Vec3(0.f, 50.f, 0.f) - TauPos).Normalize();
			setdir = true;
			TauPos += dirVector * DT * m_Speed*2 ;
			Animator2D()->Play(L"TauCharge", true);
			s_tauattack->Play(1, 0.3f, false);
		}
		else if (1.f<rest&&rest< 2.f)//1�ʰ�
		{
			readytoRush = false;
			Animator2D()->Play(L"TauWalk", true);
		}
		else if (rest > 2.f)
			rest = 0.f;
		timer = 0.f;
	}
	else if (betweenLength.x > 200.f || betweenLength.y > 200.f)
	{
		readytoRush = false;
		//timer = 0.f;
		Animator2D()->Play(L"TauWalk", true);
	}
	
	//knee kick ������ ��
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
		Vec3 dirVector = (TauPos - KneeKickpos).Normalize();//ű->���ͷ��� ���� ����
		if (kneekickTime < 0.3f)
		{
			TauPos.x += dirVector.x * DT * 10;
			TauPos.y += DT * 300;
		}
		else if (kneekickTime > 0.3f && kneekickTime < 0.6f)
		{
			TauPos.x += dirVector.x * DT * 10;
			TauPos.y -= DT * 150;
		}
		else if (kneekickTime > 0.6f && kneekickTime < 0.9f)
		{
			TauPos.x += dirVector.x * DT * 10;
			TauPos.y -= DT * 150;
		}
		else if (kneekickTime > 0.9f && kneekickTime < 1.2f)
		{
			kneekickTime = 0.f;
			isKneeKick = false;
			Animator2D()->Play(L"TauWalk", true);
		}

		Transform()->SetRelativePos(TauPos);

	}


	//�ѿ� �ǰ� �ִϸ��̼ǿ��� ���ƿ���
	if (timer > 0.4f && isHit)
	{
		isHit = false;
		Animator2D()->Play(L"TauWalk", true);
		timer = 0.f;
	}

	//�ǽð� ���� ����
	if (10.f < TauPos.z && TauPos.z < 510.f)
	{
		vector<CGameObject*> vecObj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(0)->GetParentObject();
		for (size_t i = 0; i < vecObj.size(); i++)
		{
			if (L"tree" == vecObj[i]->GetName())
			{
				Vec3 treePos = vecObj[i]->Transform()->GetRelativePos();
				Vec3 treeScale = vecObj[i]->Transform()->GetRelativeScale();
				if (TauPos.y <= treePos.y)
				{
					TauPos.z = treePos.z - 1.f;//����� �տ� ���̰�
					Transform()->SetRelativePos(TauPos);
				}
				else if (TauPos.y > treePos.y)
				{
					TauPos.z = treePos.z + 1.f;//����� �ڿ� ���̰�
					Transform()->SetRelativePos(TauPos);
				}
			}
			else if (L"tree1" == vecObj[i]->GetName())
			{
				Vec3 treePos = vecObj[i]->Transform()->GetRelativePos();
				Vec3 treeScale = vecObj[i]->Transform()->GetRelativeScale();
				if (TauPos.y <= treePos.y - treeScale.y / 2)
				{
					TauPos.z = treePos.z - 1.f;//����� �տ� ���̰�
					Transform()->SetRelativePos(TauPos);
				}
				else if (TauPos.y > treePos.y - treeScale.y / 2)
				{
					TauPos.z = treePos.z + 1.f;//����� �ڿ� ���̰�
					Transform()->SetRelativePos(TauPos);
				}
			}
			else if (L"stone" == vecObj[i]->GetName())
			{
				Vec3 stonePos = vecObj[i]->Transform()->GetRelativePos();
				if (TauPos.y <= stonePos.y)
				{
					TauPos.z = stonePos.z - 1.f;//����� �տ� ���̰�
					Transform()->SetRelativePos(TauPos);
				}
				else if (TauPos.y > stonePos.y)
				{
					TauPos.z = stonePos.z + 1.f;//����� �ڿ� ���̰�
					Transform()->SetRelativePos(TauPos);
				}
			}
		}
		vector<CGameObject*> playerobj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
		if (L"player" == playerobj[0]->GetName())
		{
			Vec3 playerPos = playerobj[0]->Transform()->GetRelativePos();
			if (TauPos.y <= playerPos.y)
			{
				TauPos.z = playerPos.z - 1.f;
				Transform()->SetRelativePos(TauPos);
			}
			else if (TauPos.y > playerPos.y)
			{
				TauPos.z = playerPos.z + 1.f;
				Transform()->SetRelativePos(TauPos);
			}
		}
		vector<CGameObject*> monobj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(3)->GetParentObject();
		for (size_t i = 0; i < monobj.size(); i++)
		{
			if (L"Bakal" == monobj[i]->GetName())
			{
				Vec3 monPos = monobj[i]->Transform()->GetRelativePos();
				if (TauPos.y <= monPos.y)
				{
					TauPos.z = monPos.z - 1.f;
					Transform()->SetRelativePos(TauPos);
				}
				else if (TauPos.y > monPos.y)
				{
					TauPos.z = monPos.z + 1.f;
					Transform()->SetRelativePos(TauPos);
				}
			}
			else if (L"goblinT" == monobj[i]->GetName())
			{
				Vec3 monPos = monobj[i]->Transform()->GetRelativePos();
				if (TauPos.y <= monPos.y)
				{
					TauPos.z = monPos.z - 1.f;
					Transform()->SetRelativePos(TauPos);
				}
				else if (TauPos.y > monPos.y)
				{
					TauPos.z = monPos.z + 1.f;
					Transform()->SetRelativePos(TauPos);
				}
			}
			else if (L"tau" == monobj[i]->GetName())
			{
				Vec3 monPos = monobj[i]->Transform()->GetRelativePos();
				if (TauPos.y <= monPos.y)
				{
					TauPos.z = monPos.z - 1.f;
					Transform()->SetRelativePos(TauPos);
				}
				else if (TauPos.y > monPos.y)
				{
					TauPos.z = monPos.z + 1.f;
					Transform()->SetRelativePos(TauPos);
				}
			}
		}
		vector<CGameObject*> skillobj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(4)->GetParentObject();
		for (size_t i = 0; i < skillobj.size(); i++)
		{
			if (L"RX78" == skillobj[i]->GetName())
			{
				Vec3 skillPos = skillobj[i]->Transform()->GetRelativePos();
				if (TauPos.y <= skillPos.y)
				{
					TauPos.z = skillPos.z - 1.f;
					Transform()->SetRelativePos(TauPos);
				}
				else if (TauPos.y > skillPos.y)
				{
					TauPos.z = skillPos.z + 1.f;
					Transform()->SetRelativePos(TauPos);
				}
			}
			else if (L"EXS" == skillobj[i]->GetName())
			{
				Vec3 skillPos = skillobj[i]->Transform()->GetRelativePos();
				if (TauPos.y <= skillPos.y)
				{
					TauPos.z = skillPos.z - 1.f;
					Transform()->SetRelativePos(TauPos);
				}
				else if (TauPos.y > skillPos.y)
				{
					TauPos.z = skillPos.z + 1.f;
					Transform()->SetRelativePos(TauPos);
				}
			}
			else if (L"DRunner" == skillobj[i]->GetName())
			{
				Vec3 skillPos = skillobj[i]->Transform()->GetRelativePos();
				if (TauPos.y <= skillPos.y)
				{
					TauPos.z = skillPos.z - 1.f;
					Transform()->SetRelativePos(TauPos);
				}
				else if (TauPos.y > skillPos.y)
				{
					TauPos.z = skillPos.z + 1.f;
					Transform()->SetRelativePos(TauPos);
				}
			}
		}
	}
	


	//Tau �̵� ����
	if (!readytoRush && !isHit && isKneeKick == false)
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
				TauPos.x += DT * m_Speed * 2 * minus;
				TauPos.y += DT * m_Speed * minus;
			}
			else if (timer > 3.f && timer < 4.f)
			{
				TauPos.x -= DT * m_Speed * 2 * minus;
				TauPos.y += DT * m_Speed * minus;
				timer = 0.f;
			}
		}
		else
		{
			if (timer < 1.f)
			{
				TauPos.x -= DT * m_Speed * 2 * minus;
				TauPos.y += DT * m_Speed * minus;
			}
			else if (timer > 3.f && timer < 4.f)
			{
				TauPos.x += DT * m_Speed * 2 * minus;
				TauPos.y -= DT * m_Speed * minus;
				timer = 0.f;
			}
		}
	}
	if(!isDead)
		Transform()->SetRelativePos(TauPos);
	//hp<0 �� Set Dead
	if (m_Hp <= 0)
	{
		m_Hp = 0;
		deadcount += DT;
		
		if (!isDead)
		{
			s_taudie->Play(1, 0.3f, false);
			SpawnPrefab(L"texture\\Prefab\\Coin.prefab", TauPos - Vec3(0.f, 50.f, 0.f));
			if (randomItem)
				SpawnPrefab(L"texture\\Prefab\\HPPotion.prefab", TauPos - Vec3(0.f, 50.f, 0.f));
			else
				SpawnPrefab(L"texture\\Prefab\\MPPotion.prefab", TauPos - Vec3(0.f, 50.f, 0.f));
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

		Animator2D()->Play(L"TauKDown", false);
		if (deadcount > 1.f)
			DestroyObject(this->GetOwner());
	}
}

void CTauScript::BeginOverlap(CCollider2D* _Other)
{
	Ptr<CSound> s_tauhit = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\monster\\tau_comm.ogg");
	vector<CGameObject*> vecplayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
	CPlayerScript* plScript = vecplayer[0]->GetScript<CPlayerScript>();
	
	if (L"bullet" == _Other->GetOwner()->GetName())
	{
		plScript->SetTarget(GetOwner()->GetID());
		s_tauhit->Play(1, 0.3f, false);
		Animator2D()->Play(L"TauHit", true);

		//�˹�
		Vec3 bulletPos = _Other->GetOwner()->Transform()->GetRelativePos();
		Vec3 dirVector = (TauPos - bulletPos).Normalize();
		TauPos.x += dirVector.x * 5;
		Transform()->SetRelativePos(TauPos);
	}
	//������ �浹��
	else if (L"EXSRB" == _Other->GetOwner()->GetName())
	{
		plScript->SetTarget(GetOwner()->GetID());
		s_tauhit->Play(1, 0.3f, false);
		EXSBullet* pScript = _Other->GetOwner()->GetScript<EXSBullet>();
		int EXSDamage = pScript->GetDamage();
		Animator2D()->Play(L"TauHit", true);

		int count = GetDigitCount(EXSDamage);
		ShowDamage(EXSDamage, count, GetOwner());
		m_Hp -= EXSDamage;
		plScript->SetComboCount();
		//�˹�
		//if (!isKnockback)//�˹���°� �ƴ� ���� �ڷ� ��������
		//{
		//	Vec3 bulletPos = _Other->GetOwner()->Transform()->GetRelativePos();
		//	Vec3 dirVector = (TauPos - bulletPos).Normalize();
		//	TauPos += dirVector * 10;
		//	Transform()->SetRelativePos(TauPos);
		//}

	}
	//�о�����
	else if (L"FastKnee" == _Other->GetOwner()->GetName())
	{
		plScript->SetTarget(GetOwner()->GetID());
		s_tauhit->Play(1, 0.3f, false);
		//�˹�
		Vec3 bulletPos = _Other->GetOwner()->Transform()->GetRelativePos();
		Vec3 dirVector = (TauPos - bulletPos).Normalize();
		int kickdistance = 20;
		for (size_t i = 0; i < 4; i++)
		{
			TauPos.x += dirVector.x * kickdistance;
			Transform()->SetRelativePos(TauPos);
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
				Animator2D()->Play(L"TauHit", true);

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
		s_tauhit->Play(1, 0.3f, false);
		//�˹�
		isKneeKick = true;

		//������
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
				Animator2D()->Play(L"TauHit", true);

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
		s_tauhit->Play(1, 0.3f, false);
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
				Animator2D()->Play(L"TauHit", true);

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
	else if (9 == _Other->GetOwner()->GetLayerIndex())//��
	{
		iswall = true;
	}
}

void CTauScript::OnOverlap(CCollider2D* _Other)
{
	vector<CGameObject*> vecplayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
	CPlayerScript* plScript = vecplayer[0]->GetScript<CPlayerScript>();
	if (L"EZ8" == _Other->GetOwner()->GetName())
	{
		plScript->SetTarget(GetOwner()->GetID());
		
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
}

void CTauScript::EndOverlap(CCollider2D* _Other)
{
	Ptr<CSound> s_tauhit = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\monster\\tau_comm.ogg");
	
	vector<CGameObject*> vecplayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
	CPlayerScript* plScript = vecplayer[0]->GetScript<CPlayerScript>();
	if (L"bullet" == _Other->GetOwner()->GetName())
	{
		CMissileScript* pScript = _Other->GetOwner()->GetScript<CMissileScript>();
		int bulletDamage = pScript->GetBulletDamage();

		int count = GetDigitCount(bulletDamage);
		ShowDamage(bulletDamage, count, GetOwner());
		m_Hp -= bulletDamage;
		isHit = true;
		plScript->SetComboCount();
	}
	else if (L"RX78" == _Other->GetOwner()->GetName())
	{
		plScript->SetTarget(GetOwner()->GetID());
		s_tauhit->Play(1, 0.3f, false);
		//������ ��� �������  ���ÿ� �����ַ���
		CRX78Script* pScript = _Other->GetOwner()->GetScript<CRX78Script>();
		int RX78Damage = pScript->GetDamage();

		//10�� (�ڸ��� ����)
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
		Animator2D()->Play(L"TauWalk", true);
	}
	else if (L"EZ8" == _Other->GetOwner()->GetName())
	{
		
		s_tauhit->Play(1, 0.3f, false);
	}
	else if (L"FastKnee" == _Other->GetOwner()->GetName())
	{
		Animator2D()->Play(L"TauWalk", true);
	}
	else if (L"Windmil" == _Other->GetOwner()->GetName())
	{
		Animator2D()->Play(L"TauWalk", true);
	}
	else if (L"player" == _Other->GetOwner()->GetName())
	{
		timer = 0.f;
		Animator2D()->Play(L"TauWalk", true);
	}
	else if (L"DRunner" == _Other->GetOwner()->GetName())
	{
		plScript->SetTarget(GetOwner()->GetID());
		//������ ��� �������  ���ÿ� �����ַ���
		CDRunnerScript* pScript = _Other->GetOwner()->GetScript<CDRunnerScript>();
		int RX78Damage = pScript->GetDamage();

		//10�� (�ڸ��� ����)
		int count = GetDigitCount(RX78Damage);
		ShowDamage(RX78Damage, count, GetOwner());
		plScript->SetComboCount();
		m_Hp -= RX78Damage;
	}
	else if (9 == _Other->GetOwner()->GetLayerIndex())//��
	{
		iswall = false;
	}
}
