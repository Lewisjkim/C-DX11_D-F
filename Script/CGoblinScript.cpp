#include "pch.h"
#include "CGoblinScript.h"
#include "CMissileScript.h"
#include "CRX78Script.h"
#include "CEZ8Script.h"
#include "EXSBullet.h"
#include "CDRunnerScript.h"


CGoblinScript::CGoblinScript()		:
	CScript((UINT)SCRIPT_TYPE::GOBLINSCRIPT),
	m_Damage(100),
	m_Speed(100.f),
	m_Hp(3000),
	m_MaxHp(3000),
	deadcount(0.f),
	m_exp(40),
	iswall(false)
{
	AddScriptParam(SCRIPT_PARAM::BOOL, &iswall, "Iswall");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_Speed, "Speed");
	AddScriptParam(SCRIPT_PARAM::INT, &m_Hp, "HP");
	AddScriptParam(SCRIPT_PARAM::INT, &m_MaxHp, "MaxHP");
	AddScriptParam(SCRIPT_PARAM::INT, &m_exp, "Exp Amount");

}

CGoblinScript::~CGoblinScript()
{
}

void CGoblinScript::begin()
{
	MeshRender()->GetDynamicMaterial();
	Animator2D()->LoadAnimation(L"anim\\GoblinHit.anim");//goblin_hit
	Animator2D()->LoadAnimation(L"anim\\GoblinKDown.anim"); //goblinKDown
	Animator2D()->LoadAnimation(L"anim\\GoblinAttack.anim");//goblinAttack
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
	isDead = false;
	Ptr<CSound> s_goblinstart = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\monster\\gbn_scared.ogg");
	s_goblinstart->Play(1, 0.4f, false);
	if (CLevelMgr::GetInst()->GetCurLevel()->GetName() == L"PracticeRoom")
	{
		m_Hp = 15000;
		m_MaxHp = 15000;
	}
}

void CGoblinScript::tick()
{
	
	random = rand();
	movetype = random <= 12000 ? true : false;
	randomItem = random <= 18000 ? true : false;

	Ptr<CSound> s_goblinhit = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\monster\\gbn_dmg_01.ogg");

	goblinPos = Transform()->GetRelativePos();
	timer += DT;
	randomTime += DT;

	if (iswall)
	{
		m_time += DT;
		timer = 0.f;
		minus *= -1;
		//iswall = false;
	}

	//�� �浹�� �� ����� �ӵ��� �ʱ�ȭ
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
	isLeft = goblinPos.x < playerPos.x ? true : false;
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

	//�÷��̾� ����
	Vec3 dirVector = (playerPos - goblinPos).Normalize();
	Vec3 betweenLength = Vec3(0.f, 0.f, 0.f);
	betweenLength.x = abs(playerPos.x - goblinPos.x);
	betweenLength.y = abs(playerPos.y - goblinPos.y);
	if (iswall == false && isKneeKick == false && 4.f < betweenLength.x && betweenLength.x <= 200.f && 4.f < betweenLength.y && betweenLength.y <= 200.f)
	{
		readytoAttack = true;
		goblinPos += dirVector * DT * m_Speed; // �÷��̾� �������� �̵�
	}
	else if (isKneeKick == false && betweenLength.x <= 4.f && betweenLength.y <= 4.f)
	{
		readytoAttack = true;
	}
	else if (betweenLength.x > 200.f || betweenLength.y > 200.f)
	{
		readytoAttack = false;
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
		Vec3 dirVector = (goblinPos - KneeKickpos).Normalize();//ű->���ͷ��� ���� ����
		if (kneekickTime < 0.3f)
		{
			goblinPos.x += dirVector.x * DT * 10;
			goblinPos.y += DT * 300;
		}
		else if (kneekickTime > 0.3f && kneekickTime < 0.6f)
		{
			goblinPos.x += dirVector.x * DT * 10;
			goblinPos.y -= DT * 150;
		}
		else if (kneekickTime > 0.6f && kneekickTime < 0.9f)
		{
			goblinPos.x += dirVector.x * DT * 10;
			goblinPos.y -= DT * 150;

		}
		else if (kneekickTime > 0.9f && kneekickTime < 1.2f)
		{
			/*goblinPos.x += dirVector.x * DT * 10;
			goblinPos.y -= DT * 50;*/
			kneekickTime = 0.f;
			isKneeKick = false;
			Animator2D()->Play(L"GoblinWalk", true);
		}

		Transform()->SetRelativePos(goblinPos);

	}

	//�ѿ� �ǰ� �ִϸ��̼ǿ��� ���ƿ���
	if (timer > 0.4f && isHit)
	{
		isHit = false;
		Animator2D()->Play(L"GoblinWalk", true);
		timer = 0.f;
	}

	// �ǽð� ���� ����
	vector<CGameObject*> vecObj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(0)->GetParentObject();
	for (size_t i = 0; i < vecObj.size(); i++)
	{
		if (L"tree" == vecObj[i]->GetName())
		{
			Vec3 treePos = vecObj[i]->Transform()->GetRelativePos();
			Vec3 treeScale = vecObj[i]->Transform()->GetRelativeScale();
			if (goblinPos.y <= treePos.y)
			{
				goblinPos.z = treePos.z - 1.f;//����� �տ� ���̰�
				Transform()->SetRelativePos(goblinPos);
			}
			else if (goblinPos.y > treePos.y)
			{
				goblinPos.z = treePos.z + 1.f;//����� �ڿ� ���̰�
				Transform()->SetRelativePos(goblinPos);
			}
		}
		else if (L"tree1" == vecObj[i]->GetName())
		{
			Vec3 treePos = vecObj[i]->Transform()->GetRelativePos();
			Vec3 treeScale = vecObj[i]->Transform()->GetRelativeScale();
			if (goblinPos.y <= treePos.y - treeScale.y / 2)
			{
				goblinPos.z = treePos.z - 1.f;//����� �տ� ���̰�
				Transform()->SetRelativePos(goblinPos);
			}
			else if (goblinPos.y > treePos.y - treeScale.y / 2)
			{
				goblinPos.z = treePos.z + 1.f;//����� �ڿ� ���̰�
				Transform()->SetRelativePos(goblinPos);
			}
		}
		else if (L"stone" == vecObj[i]->GetName())
		{
			Vec3 stonePos = vecObj[i]->Transform()->GetRelativePos();
			if (goblinPos.y <= stonePos.y)
			{
				goblinPos.z = stonePos.z - 1.f;//����� �տ� ���̰�
				Transform()->SetRelativePos(goblinPos);
			}
			else if (goblinPos.y > stonePos.y)
			{
				goblinPos.z = stonePos.z + 1.f;//����� �ڿ� ���̰�
				Transform()->SetRelativePos(goblinPos);
			}
		}
	}
	vector<CGameObject*> playerobj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
	if (L"player" == playerobj[0]->GetName())
	{
		Vec3 playerPos = playerobj[0]->Transform()->GetRelativePos();
		if (goblinPos.y <= playerPos.y)
		{
			goblinPos.z = playerPos.z - 1.f;
			Transform()->SetRelativePos(goblinPos);
		}
		else if (goblinPos.y > playerPos.y)
		{
			goblinPos.z = playerPos.z + 1.f;
			Transform()->SetRelativePos(goblinPos);
		}
	}
	vector<CGameObject*> monobj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(3)->GetParentObject();
	for (size_t i = 0; i < monobj.size(); i++)
	{
		if (L"Bakal" == monobj[i]->GetName())
		{
			Vec3 monPos = monobj[i]->Transform()->GetRelativePos();
			if (goblinPos.y <= monPos.y)
			{
				goblinPos.z = monPos.z - 1.f;
				Transform()->SetRelativePos(goblinPos);
			}
			else if (goblinPos.y > monPos.y)
			{
				goblinPos.z = monPos.z + 1.f;
				Transform()->SetRelativePos(goblinPos);
			}
		}
		else if (L"goblinT" == monobj[i]->GetName())
		{
			Vec3 monPos = monobj[i]->Transform()->GetRelativePos();
			if (goblinPos.y <= monPos.y)
			{
				goblinPos.z = monPos.z - 1.f;
				Transform()->SetRelativePos(goblinPos);
			}
			else if (goblinPos.y > monPos.y)
			{
				goblinPos.z = monPos.z + 1.f;
				Transform()->SetRelativePos(goblinPos);
			}
		}
		else if (L"tau" == monobj[i]->GetName())
		{
			Vec3 monPos = monobj[i]->Transform()->GetRelativePos();
			if (goblinPos.y <= monPos.y)
			{
				goblinPos.z = monPos.z - 1.f;
				Transform()->SetRelativePos(goblinPos);
			}
			else if (goblinPos.y > monPos.y)
			{
				goblinPos.z = monPos.z + 1.f;
				Transform()->SetRelativePos(goblinPos);
			}
		}
	}
	vector<CGameObject*> skillobj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(4)->GetParentObject();
	for (size_t i = 0; i < skillobj.size(); i++)
	{
		if (L"RX78" == skillobj[i]->GetName())
		{
			Vec3 skillPos = skillobj[i]->Transform()->GetRelativePos();
			if (goblinPos.y <= skillPos.y)
			{
				goblinPos.z = skillPos.z - 1.f;
				Transform()->SetRelativePos(goblinPos);
			}
			else if (goblinPos.y > skillPos.y)
			{
				goblinPos.z = skillPos.z + 1.f;
				Transform()->SetRelativePos(goblinPos);
			}
		}
		else if (L"EXS" == skillobj[i]->GetName())
		{
			Vec3 skillPos = skillobj[i]->Transform()->GetRelativePos();
			if (goblinPos.y <= skillPos.y)
			{
				goblinPos.z = skillPos.z - 1.f;
				Transform()->SetRelativePos(goblinPos);
			}
			else if (goblinPos.y > skillPos.y)
			{
				goblinPos.z = skillPos.z + 1.f;
				Transform()->SetRelativePos(goblinPos);
			}
		}
		else if (L"DRunner" == skillobj[i]->GetName())
		{
			Vec3 skillPos = skillobj[i]->Transform()->GetRelativePos();
			if (goblinPos.y <= skillPos.y)
			{
				goblinPos.z = skillPos.z - 1.f;
				Transform()->SetRelativePos(goblinPos);
			}
			else if (goblinPos.y > skillPos.y)
			{
				goblinPos.z = skillPos.z + 1.f;
				Transform()->SetRelativePos(goblinPos);
			}
		}
	}
	//goblin �̵� ����
	if (!readytoAttack && !isHit && isKneeKick == false)
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
				goblinPos.x += DT * m_Speed * 2 * minus;
				goblinPos.y += DT * m_Speed * minus;
			}
			else if (timer > 3.f && timer < 4.f)
			{
				goblinPos.x -= DT * m_Speed * 2 * minus;
				goblinPos.y += DT * m_Speed * minus;
				timer = 0.f;
			}
		}
		else
		{
			if (timer < 1.f)
			{
				goblinPos.x -= DT * m_Speed * 2 * minus;
				goblinPos.y += DT * m_Speed * minus;
			}
			else if (timer > 3.f && timer < 4.f)
			{
				goblinPos.x += DT * m_Speed * 2 * minus;
				goblinPos.y -= DT * m_Speed * minus;
				timer = 0.f;
			}
		}
	}
	if (!isDead)
		Transform()->SetRelativePos(goblinPos);
	//hp<0 �� Set Dead
	if (m_Hp <= 0)
	{

		m_Hp = 0;
		deadcount += DT;

		if (!isDead)
		{
			s_goblinhit->Play(1, 0.4f, false);
			SpawnPrefab(L"texture\\Prefab\\Coin.prefab", goblinPos);
			if (randomItem)
				SpawnPrefab(L"texture\\Prefab\\HPPotion.prefab", goblinPos);
			else
				SpawnPrefab(L"texture\\Prefab\\MPPotion.prefab", goblinPos);
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
		Animator2D()->Play(L"goblinKDown", false);
		if (deadcount > 1.f)
		{
			DestroyObject(this->GetOwner());
		}

	}
	
}

void CGoblinScript::BeginOverlap(CCollider2D* _Other)
{
	Ptr<CSound> s_goblinhit = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\monster\\gbn_dmg_01.ogg");
	vector<CGameObject*> vecplayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
	CPlayerScript* plScript = vecplayer[0]->GetScript<CPlayerScript>();
	if (L"bullet" == _Other->GetOwner()->GetName())
	{
		Animator2D()->Play(L"goblin_hit", true);
		s_goblinhit->Play(1, 0.4f, false);
		plScript->SetTarget(GetOwner()->GetID());
		Vec3 bulletPos = _Other->GetOwner()->Transform()->GetRelativePos();
		Vec3 dirVector = ( goblinPos - bulletPos).Normalize();
		goblinPos += dirVector * 5;
		Transform()->SetRelativePos(goblinPos);
	}
	//������ �浹��
	else if (L"EXSRB" == _Other->GetOwner()->GetName())
	{
		s_goblinhit->Play(1, 0.4f, false);
		EXSBullet* pScript = _Other->GetOwner()->GetScript<EXSBullet>();
		int EXSDamage = pScript->GetDamage();
		Animator2D()->Play(L"goblin_hit", true);
		plScript->SetTarget(GetOwner()->GetID());
		int count = GetDigitCount(EXSDamage);
		ShowDamage(EXSDamage, count, GetOwner());
		plScript->SetComboCount();
		m_Hp -= EXSDamage;
	}
	//�о�����
	else if (L"FastKnee" == _Other->GetOwner()->GetName())
	{
		s_goblinhit->Play(1, 0.4f, false);
		plScript->SetTarget(GetOwner()->GetID());
		//�˹�
		Vec3 bulletPos = _Other->GetOwner()->Transform()->GetRelativePos();
		Vec3 dirVector = (goblinPos - bulletPos).Normalize();
		int kickdistance = 20;
		for (size_t i = 0; i < 4; i++)
		{
			goblinPos.x += dirVector.x * kickdistance;
			Transform()->SetRelativePos(goblinPos);
			kickdistance-3;
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
				Animator2D()->Play(L"goblin_hit", true);

				int count = GetDigitCount(FastKneeDamage);
				ShowDamage(FastKneeDamage, count, GetOwner());
				plScript->SetComboCount();
				m_Hp -= FastKneeDamage;
			}
		}
	}
	else if (L"KneeKick" == _Other->GetOwner()->GetName())
	{
		s_goblinhit->Play(1, 0.4f, false);
		plScript->SetTarget(GetOwner()->GetID());
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
				Animator2D()->Play(L"goblin_hit", true);

				int count = GetDigitCount(KneeKickDamage);
				ShowDamage(KneeKickDamage, count, GetOwner());
				plScript->SetComboCount();
				m_Hp -= KneeKickDamage;
			}
		}
	}
	else if (L"Windmil" == _Other->GetOwner()->GetName()) 
	{
		plScript->SetTarget(GetOwner()->GetID());
		s_goblinhit->Play(1, 0.4f, false);
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
				Animator2D()->Play(L"goblin_hit", true);

				int count = GetDigitCount(WindmilDamage);
				for (size_t i = 0; i < 2; i++)
				{
					ShowDamage(WindmilDamage, count, GetOwner());
					plScript->SetComboCount();
					m_Hp -= WindmilDamage;
				}
			}
		}
	}
	else if (9 == _Other->GetOwner()->GetLayerIndex())//��
	{

		iswall = true;
	}
}

void CGoblinScript::OnOverlap(CCollider2D* _Other)
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
			plScript->SetComboCount();
			m_Hp -= EZ8Damage;
		}
	}
	else if (L"player" == _Other->GetOwner()->GetName())
	{
		attackTerm += DT;
		if (attackTerm < 0.5f)
		{
			Animator2D()->Play(L"goblinAttack", false);
			Animator2D()->GetCurAnim()->Reset();
		}
		else if (attackTerm > 1.5f)
		{
			Animator2D()->Play(L"goblinAttack", false);
			Animator2D()->GetCurAnim()->Reset();
			attackTerm = 0.f;
		}
		
	}
}

void CGoblinScript::EndOverlap(CCollider2D* _Other)
{
	vector<CGameObject*> vecplayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
	CPlayerScript* plScript = vecplayer[0]->GetScript<CPlayerScript>();
	if (L"bullet" == _Other->GetOwner()->GetName())
	{
		CMissileScript* pScript = _Other->GetOwner()->GetScript<CMissileScript>();
		int bulletDamage = pScript->GetBulletDamage();
		
		int count = GetDigitCount(bulletDamage);
		ShowDamage(bulletDamage, count, GetOwner());
		plScript->SetComboCount();
		m_Hp -= bulletDamage;
		isHit = true;
	}
	 else if (L"RX78" == _Other->GetOwner()->GetName())
	{
		plScript->SetTarget(GetOwner()->GetID());
		Ptr<CSound> s_goblinhit = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\monster\\gbn_dmg_01.ogg");
		s_goblinhit->Play(1, 0.3f, false);
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
		 Animator2D()->Play(L"GoblinWalk", true);
	 }
	 else if (L"EZ8"== _Other->GetOwner()->GetName())
	{
		Ptr<CSound> s_goblinhit = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\monster\\gbn_dmg_01.ogg");
		s_goblinhit->Play(1, 0.3f, false);
	}
	 else if (L"FastKnee" == _Other->GetOwner()->GetName())
	 {
		 Animator2D()->Play(L"GoblinWalk", true);
	 }
	 else if (L"Windmil" == _Other->GetOwner()->GetName())
	 {
		 Animator2D()->Play(L"GoblinWalk", true);
	 }
	 else if (L"player" == _Other->GetOwner()->GetName())
	{
		timer = 0.f;
		Animator2D()->Play(L"GoblinWalk", true);
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





