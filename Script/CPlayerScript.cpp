#include "pch.h"
#include "CPlayerScript.h"
#include <Engine\CEngine.h>
#include <Engine\CEventMgr.h>
#include <Engine\CMeshRender.h>
#include <Engine\components.h>
#include <Engine\CMaterial.h>
#include <Engine\CAnim2D.h>
#include <Engine/CGameObject.h>
#include <Engine/CFontMgr.h>
#include "CMissileScript.h"
#include "CRX78Script.h"
#include "CEZ8Script.h"
#include "CEXSScript.h"
#include "CDRunnerScript.h"
#include "CGTStoneScript.h"
#include "CGTStoneScript2.h"
#include "CGoblinScript.h"
#include "CTauScript.h"
#include "CItemScript.h"
#include "CBakalScript.h"
#include "CBakalAttackScript.h"
#include "/DirectX_Practice/DirectX_/Project/Client/CLevelSaveLoad.h"

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT),
	level(0),
	hp(0),
	maxhp(0),
	mp(0),
	maxmp(0),
	exp(0),
	coin(0),
	hpPotion(0),
	mpPotion(0),
	maxexp(0),
	fatigue(10),
	PAttack(50),
	MAttack(90),
	m_fSpeed(100.f),
	gravityAccel(500.f)
{
	AddScriptParam(SCRIPT_PARAM::INT, &coin, "Coin");
	AddScriptParam(SCRIPT_PARAM::INT, &hpPotion, "HP_Potion");
	AddScriptParam(SCRIPT_PARAM::INT, &mpPotion, "MP_Potion");
	AddScriptParam(SCRIPT_PARAM::INT, &hp, "HP");
	AddScriptParam(SCRIPT_PARAM::INT, &maxhp, "MaxHP");
	AddScriptParam(SCRIPT_PARAM::INT, &mp, "MP");
	AddScriptParam(SCRIPT_PARAM::INT, &maxmp, "MaxMP");
	AddScriptParam(SCRIPT_PARAM::INT, &exp, "EXP");
	AddScriptParam(SCRIPT_PARAM::INT, &fatigue, "Fatigue");
	AddScriptParam(SCRIPT_PARAM::INT, &level, "Level");
	AddScriptParam(SCRIPT_PARAM::INT, &PAttack, "PAttack");
	AddScriptParam(SCRIPT_PARAM::INT, &MAttack, "MAttack");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_fSpeed, "MoveSpeed");
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::begin()
{
	MeshRender()->GetDynamicMaterial();
	Animator2D()->LoadAnimation(L"anim\\GunnerWalk.anim");
	Animator2D()->LoadAnimation(L"anim\\GunnerRun.anim");
	Animator2D()->LoadAnimation(L"anim\\GunnerJump.anim");
	Animator2D()->LoadAnimation(L"anim\\GunnerSlide.anim");
	Animator2D()->LoadAnimation(L"anim\\GunnerAttack.anim");
	Animator2D()->LoadAnimation(L"anim\\GunnerHit.anim");
	Animator2D()->LoadAnimation(L"anim\\GunnerKDown.anim");
	Animator2D()->LoadAnimation(L"anim\\GunnerStandup.anim");
	Animator2D()->LoadAnimation(L"anim\\GunnerKneeKick.anim");
	Animator2D()->LoadAnimation(L"anim\\GunnerFastKnee.anim");
	Animator2D()->LoadAnimation(L"anim\\GunnerWindmil.anim");
	Animator2D()->LoadAnimation(L"anim\\FastKnee.anim");
	
	vector<int> previnfo = CLevelMgr::GetInst()->GetPlayerInfo();
	level = previnfo[0];//(1),
	hp = previnfo[1];//500)
	maxhp = previnfo[2];//(500),
	mp = previnfo[3];//(500),
	maxmp = previnfo[4];//(500),
	exp = previnfo[5];//(0),
	coin = previnfo[6];
	hpPotion = previnfo[7];
	mpPotion = previnfo[8];
	fatigue = previnfo[9];
	maxexp = 100;//(100),

	vector<CGameObject*> vecmonster = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(3)->GetParentObject();
	if (vecmonster.size() != 0)
		--fatigue;

	readytorun = false;
	isrunning = false;
	iswalking = true;
	isjump = false;
	fastkneecount = 0.f;
	kneekickcount = 0.f;
	windmilcount = 0.f;
	knockdownCount = 0.f;
	m_time = 0.f;
	StartTime = 0.f;
	ishitCount = 0.f;
	goblinattackTerm = 0.f;
	tauattackTerm = 0.f;
	isRush = false;
	isItem = false;
	BakalattackTerm = 0.f;
	BakalstampTerm = 0.f;
	rest = false;
	comboCount = 0;
	ComboStart = 0.f;
}

void CPlayerScript::tick()
{
	Ptr<CSound> s_walk = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunner\\bnf2_foot_village_01.ogg");
	Ptr<CSound> s_shoot = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunner\\45calibera_01.ogg");
	Ptr<CSound> s_hit = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunner\\gn_dmg_02.ogg");
	Ptr<CSound> s_jump = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunner\\gn_jump.ogg");
	Ptr<CSound> s_laugh = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunner\\gn_laugh.ogg");
	Ptr<CSound> s_die = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunner\\gn_die_02.ogg");
	Ptr<CSound> s_slide = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunner\\gn_sliding_01.ogg");
	Ptr<CSound> s_hitobj = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunner\\hit_wood_01.ogg");
	Ptr<CSound> s_hprec = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunner\\hp_recovered.ogg");
	Ptr<CSound> s_mprec = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunner\\mp_recovered.ogg");
	Ptr<CSound> s_itempick = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunner\\item_pick.ogg");

	Ptr<CSound> s_ready  = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunnerskill\\gn_cdown_02.ogg");
	Ptr<CSound> s_countdown = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunnerskill\\gn_cdown_01.ogg");
	Ptr<CSound> s_levelup = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\ui_c_levelup.ogg");
	//skills
	//DRunner
	Ptr<CSound> s_DRstart = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunnerskill\\gn_dimension_runner.ogg");
	//rx78
	Ptr<CSound> s_RX78 = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunnerskill\\gn_lrunner.ogg");
	
	//knee kick &fast kick
	Ptr<CSound> s_Z_Fast = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunnerskill\\gn_jspike_01.ogg");
	//windmil
	Ptr<CSound> s_Windmil = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\gunnerskill\\p_windmill.ogg");
	
	/*s_walk->Play(0, 0.5f, false);*/
	Vec3 vCurPos = Transform()->GetRelativePos();
	wstring gameID = CEngine::GetInst()->GetGameID();//게임 아이디
	
	if(isknockdown == true)
		knockdownCount += DT;
	
	//level
	if (exp == maxexp)
	{
		level = level+1;
		exp = 0;
		hp = maxhp;
		mp = maxmp;
		SpawnPrefab(L"texture\\Prefab\\Levelup.prefab", vCurPos + Vec3(30.f, 100.f, 0.f), 0.5f);
		s_levelup->Play(1, 0.5f, false);
	}
	else if (exp > maxexp)
	{
		int extraExp = exp - maxexp;
		exp = extraExp;
		level = level + 1;
		hp = maxhp;
		mp = maxmp;
		SpawnPrefab(L"texture\\Prefab\\Levelup.prefab", vCurPos + Vec3(30.f, 100.f, 0.f), 0.5f);
		s_levelup->Play(1, 0.5f, false);
	}

	m_time += DT; // 프레임당 걸리는 시간
	

	//피격시
	if (ishit)
	{
		ishitCount += DT;
		if (0.3f<ishitCount && ishitCount < 0.6f)
		{
			s_hit->Play(1, 0.5f, false);
			Animator2D()->Play(L"GunnerHit", false);
			Animator2D()->GetCurAnim()->Reset();
		}
		
		if (ishitCount > 1.3f)
		{
			ishit = false;
			ishitCount = 0.f;
		}
	}

	//이동================
	if (knockdownCount > 0.8f || knockdownCount == 0) //쓰러지기 전이나 쓰러지고 2초후에만 이동 가능
	{
		if(Animator2D()->GetCurAnim()->GetName()!=L"GunnerStandup"
			&& Animator2D()->GetCurAnim()->GetName() != L"GunnerAttack"
			&& Animator2D()->GetCurAnim()->GetName() != L"GunnerKneeKick"
			&& Animator2D()->GetCurAnim()->GetName() != L"GunnerFastKnee"
			&& Animator2D()->GetCurAnim()->GetName() != L"GunnerWindmil"
			&& Animator2D()->GetCurAnim()->GetName() != L"FastKnee"
			&&ishit ==false
			&&isRush == false
			&& isItem == false)
			Animator2D()->Play(L"GunnerIdle", true);
		if (KEY_PRESSED(KEY::UP))
		{
			vCurPos.y += DT * m_fSpeed * 2;
			s_walk->Play(0, 0.5f, false);
			Animator2D()->Play(L"GunnerWalk", true);
			if (iswall)
			{
				vCurPos.y -= DT * m_fSpeed * 7;
				iswall = false;
			}
		}
		if (KEY_RELEASE(KEY::UP))
		{
			s_walk->Stop();
			Animator2D()->Play(L"GunnerIdle", true);
		}

		if (KEY_PRESSED(KEY::DOWN))
		{
			vCurPos.y -= DT * m_fSpeed * 2;
			s_walk->Play(0, 0.5f, false);
			Animator2D()->Play(L"GunnerWalk", true);
			if (iswall)
			{
				vCurPos.y += DT * m_fSpeed * 7;
				iswall = false;
			}
		}
		if (KEY_RELEASE(KEY::DOWN))
		{
			s_walk->Stop();
			Animator2D()->Play(L"GunnerIdle", true);
		}

		if (KEY_PRESSED(KEY::LEFT))
		{
			Vec3 rot = Vec3(0.f, -180.f, 0.f);
			rot = (rot / 180.f) * XM_PI;
			s_walk->Play(0, 0.5f, false);
			Animator2D()->Play(L"GunnerWalk", true);
			if (StartTime == 0 || StartTime >= 0.3f)
			{
				m_time = 0.f;
				StartTime = m_time;
				readytorun = true;
				vCurPos.x -= DT * m_fSpeed * 2;
				Transform()->SetRelativeRot(rot);
				m_Dir = false;
				s_walk->Play(1, 0.5f, false);
				Animator2D()->Play(L"GunnerWalk", true);
				if (iswall)
				{
					vCurPos.x += DT * m_fSpeed * 5;
				}
			}

			if (StartTime < 0.3f && readytorun && !iswalking)
			{
				vCurPos.x -= DT * m_fSpeed * 3;
				Transform()->SetRelativeRot(rot);
				m_Dir = false;
				s_walk->Play(1, 0.5f, false);
				Animator2D()->Play(L"GunnerRun", true);
				isrunning = true;
				if (iswall)
				{
					vCurPos.x += DT * m_fSpeed * 8;
				}
				if (KEY_PRESSED(KEY::X))
				{
					s_walk->Stop();
					slidetime += DT;
					if (0.f<slidetime&&slidetime<0.1f)
					{
						s_slide->Play(1, 0.5f, false);
					}
					if (slidetime < 1.f)
					{
						Animator2D()->Play(L"GunnerSlide", true);
					}
					else if (slidetime > 1.f)
					{
						s_walk->Play(0, 0.5f, false);
						Animator2D()->Play(L"GunnerRun", true);
					}
					if (iswall)
					{
						vCurPos.x += 5;
					}
				}
				if (KEY_RELEASE(KEY::X))
				{
					s_slide->Stop();
					slidetime = 0.f;
				}
			}
			iswall = false;
		}
		if (KEY_RELEASE(KEY::LEFT))
		{
			if (isrunning)
			{
				isrunning = false;
				readytorun = false;
				iswalking = true;
				StartTime = 0.f;
			}
			if (readytorun)
			{
				iswalking = false;
				int a = 0;
			}
			s_walk->Stop();
			s_slide->Stop();
			Animator2D()->Play(L"GunnerIdle", true);
		}

		if (KEY_PRESSED(KEY::RIGHT))
		{
			if (StartTime == 0 || StartTime >= 0.3f)
			{
				m_time = 0.f;
				StartTime = m_time;
				readytorun = true;

				//Press 시 - 걷기
				vCurPos.x += DT * m_fSpeed * 2;
				Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));
				m_Dir = true;
				s_walk->Play(1, 0.5f, false);
				Animator2D()->Play(L"GunnerWalk", true);
				if (iswall)
				{
					vCurPos.x -= DT * m_fSpeed * 5;
				}
			}
			if (StartTime < 0.3f && readytorun && !iswalking)
			{
				vCurPos.x += DT * m_fSpeed * 3;
				Transform()->SetRelativeRot(Vec3(0.f, 0.f, 0.f));
				m_Dir = true;
				s_walk->Play(1, 0.5f, false);
				Animator2D()->Play(L"GunnerRun", true);
				isrunning = true;
				if (iswall)
				{
					vCurPos.x -= DT * m_fSpeed * 8;
				}
				if (KEY_PRESSED(KEY::X))
				{
					s_walk->Stop();
					slidetime += DT;
					if (0.f < slidetime && slidetime < 0.1f)
					{
						s_slide->Play(1, 0.5f, false);
					}
					if (slidetime < 1.f)
					{
						Animator2D()->Play(L"GunnerSlide", true);
					}
					else if (slidetime > 1.f)
					{
						s_walk->Play(1, 0.5f, false);
						Animator2D()->Play(L"GunnerRun", true);
					}
					if (iswall)
					{
						vCurPos.x -= 5;
					}
				}
				if (KEY_RELEASE(KEY::X))
				{
					s_slide->Stop();
					slidetime = 0.f;
				}
			}
			iswall = false;
		}
		if (KEY_RELEASE(KEY::RIGHT))
		{
			if (isrunning)
			{
				isrunning = false;
				readytorun = false;
				iswalking = true;
				StartTime = 0.f;
			}
			if (readytorun)
			{
				iswalking = false;
				int a = 0;
			}
			s_walk->Stop();
			s_slide->Stop();
			Animator2D()->Play(L"GunnerIdle", true);
		}

		isknockdown = false; //쓰러지고 2초가 지나거나 쓰러지지 않은 상태에서는 false;
		knockdownCount = 0.f;
	}
	
	//점프================
	if (KEY_PRESSED(KEY::C))//중력 및 rigidbody 코드 작성 후 진행하기
	{
		if (isknockdown == true) //쓰러져 있을 때 c를 누르면 퀵 스탠드
		{
			
			standupCount += DT;
			Animator2D()->Play(L"GunnerStandup", false);
			if (standupCount > 10.f)
			{
				isknockdown = false;
				knockdownCount = 0.f;
			}
		}
		//점프
		else if(isknockdown ==false|| isknockdown == false && KEY_PRESSED(KEY::RIGHT)|| isknockdown == false && KEY_PRESSED(KEY::LEFT))
		{
			s_walk->Stop();
			isjump = true;
			jumpStartTime += DT;
			if (0.f < jumpStartTime && jumpStartTime < 0.1f)
			{
				s_jump->Play(1, 0.5f, false);
			}
			
			Animator2D()->Play(L"GunnerJump", false);
			Animator2D()->GetCurAnim()->Reset();
			jumppointY = vCurPos.y;
			//isjump = true;
			jumpmaxpointY = jumppointY + 150.f;
			if (jumpStartTime < 0.2f)
				vCurPos.y += gravityAccel * 1.5f * DT;
			else if (0.2f < jumpStartTime&& jumpStartTime < 0.4f && vCurPos.y >=jumppointY)
			{
				vCurPos.y -= gravityAccel / 2 * DT;
			}
			else if (0.4f < jumpStartTime && jumpStartTime < 0.6f && vCurPos.y >= jumppointY)
			{
				vCurPos.y -= gravityAccel / 2 * DT;
			}
			else if (0.6f < jumpStartTime && jumpStartTime < 0.8f && vCurPos.y >= jumppointY)
			{
				vCurPos.y -= gravityAccel / 2 * DT;
			}
			else if (jumpStartTime > 0.8)
			{
				vCurPos.y = jumppointY;
				if (KEY_PRESSED(KEY::RIGHT) || KEY_PRESSED(KEY::LEFT))
				{
					s_walk->Play(1, 0.5f, false);
					Animator2D()->Play(L"GunnerRun", true);
				}
				else
				{
					Animator2D()->Play(L"GunnerIdle", true);
				}
			}
		}
	}
	if (KEY_RELEASE(KEY::C))
	{
		if (isjump == true)
		{
			jumpStartTime = 0.f;
			isjump = false;
		}
		else
		{
			standupCount = 0;
			Animator2D()->Play(L"GunnerIdle", true);
		}
	}
	
	//기본 공격 
	ComboStart += DT;
	if (KEY_TAP(KEY::X)&&KEY_NONE(KEY::RIGHT)&&KEY_NONE(KEY::LEFT)&& isItem==false)
	{
		s_shoot->Stop();
		Animator2D()->Play(L"GunnerAttack", true);
		//Shoot();
		if (ComboStart < 1.f)
		{
			s_shoot->Play(1, 0.5f, false);
			Shoot();
		}
		ComboStart = 0.f;
	}
	if (KEY_RELEASE(KEY::X))
	{
		ComboStart += DT;
		if (ComboStart > 0.005)
			Animator2D()->Play(L"GunnerIdle", true);
	}

	//skill===================== 
	if (KEY_TAP(KEY::A) && mp != 0)
	{
		Animator2D()->Play(L"GunnerStandup", false);
		s_RX78->Play(1, 0.5f, false);
		mp -= 10;
		RX78();
	}
	if (KEY_RELEASE(KEY::A))
	{
		Animator2D()->Play(L"GunnerIdle", true);
	}
	if (KEY_TAP(KEY::S) && mp != 0)
	{
		Animator2D()->Play(L"GunnerStandup", false);
		EZ8();
	}
	if (KEY_RELEASE(KEY::S))
	{
		Animator2D()->Play(L"GunnerIdle", true);
	}
	if (KEY_TAP(KEY::D) && mp != 0)
	{
		EXS();
	}
	if (KEY_TAP(KEY::R) && mp != 0)
	{
		s_DRstart->Play(1, 0.4f, false);
		DRunner();
	}
	if (KEY_TAP(KEY::F) && mp != 0)
	{
		s_Z_Fast->Play(1, 0.3f, false);
		FastKnee();
	}
	if (KEY_RELEASE(KEY::F))
	{
		Animator2D()->Play(L"GunnerIdle", true);
	}
	if (KEY_TAP(KEY::Z) && mp != 0)
	{
		s_Z_Fast->Play(1, 0.3f, false);
		Kneekick();
	}
	if (KEY_RELEASE(KEY::Z))
	{
		Animator2D()->Play(L"GunnerIdle", true);
	}
	if (KEY_TAP(KEY::E) && mp != 0)
	{
		s_Windmil->Play(1, 0.3f, false);
		Animator2D()->Play(L"GunnerWindmil", false);
		Windmil();
	}
	if (KEY_RELEASE(KEY::E))
	{
		Animator2D()->Play(L"GunnerIdle", true);
	}

	if (KEY_TAP(KEY::J)&& hpPotion != 0)
	{
		hp += 100;
		if (hp > maxmp)
			hp == maxmp;
		hpPotion -= 1;
		s_hprec->Play(1, 0.3f, false);
		SpawnPrefab(L"texture\\Prefab\\HPPotionEffect.prefab", vCurPos + Vec3(0.f,0.f,-500.f), 1.f);
	}
	if (KEY_TAP(KEY::K) && mpPotion != 0)
	{
		mp += 100;
		if (mp > maxmp)
			mp == maxmp;
		mpPotion -= 1;
		s_mprec->Play(1, 0.3f, false);
		SpawnPrefab(L"texture\\Prefab\\MPPotionEffect.prefab", vCurPos + Vec3(0.f, 0.f, -500.f), 1.f);
	}
	if (KEY_TAP(KEY::N) )
	{
		hp -= 100;
		
	}
	if (KEY_TAP(KEY::M))
	{
		mp -= 100;
		
	}
	if (KEY_TAP(KEY::L))
	{
		exp += 100;
	}
	
	// 실시간 깊이 변경
	vector<CGameObject*> vecObj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(0)->GetParentObject();
	for (size_t i = 0; i < vecObj.size(); i++)
	{
		if (L"tree" == vecObj[i]->GetName())
		{
			Vec3 treePos = vecObj[i]->Transform()->GetRelativePos();
			Vec3 treeScale = vecObj[i]->Transform()->GetRelativeScale();
			if (vCurPos.y <= treePos.y)
			{
				treePos.z = vCurPos.z + 1.f;//뒤에 보이게
				vecObj[i]->Transform()->SetRelativePos(treePos);
			}
			else if (vCurPos.y > treePos.y)
			{
				treePos.z = vCurPos.z - 1.f;//앞에보이게
				vecObj[i]->Transform()->SetRelativePos(treePos);
			}
		}
		else if (L"tree1" == vecObj[i]->GetName())
		{
			Vec3 treePos = vecObj[i]->Transform()->GetRelativePos();
			Vec3 treeScale = vecObj[i]->Transform()->GetRelativeScale();
			if (vCurPos.y <= treePos.y - treeScale.y/2)
			{
				treePos.z = vCurPos.z + 1.f;//뒤에 보이게
				vecObj[i]->Transform()->SetRelativePos(treePos);
			}
			else if (vCurPos.y > treePos.y - treeScale.y / 2)
			{
				treePos.z = vCurPos.z - 1.f;//앞에보이게
				vecObj[i]->Transform()->SetRelativePos(treePos);
			}
		}
		else if (L"stone" == vecObj[i]->GetName())
		{
			Vec3 stonePos = vecObj[i]->Transform()->GetRelativePos();
			if (vCurPos.y <= stonePos.y)
			{
				stonePos.z = vCurPos.z + 1.f;//뒤에 보이게
				vecObj[i]->Transform()->SetRelativePos(stonePos);
			}
			else if (vCurPos.y > stonePos.y)
			{
				stonePos.z = vCurPos.z - 1.f;//앞에보이게
				vecObj[i]->Transform()->SetRelativePos(stonePos);
			}
		}
	}
	vector<CGameObject*> breakObj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(8)->GetParentObject();
	for (size_t i = 0; i < breakObj.size(); i++)
	{
		if (L"h_obj1" == breakObj[i]->GetName())
		{
			Vec3 treePos = breakObj[i]->Transform()->GetRelativePos();
			Vec3 treeScale = breakObj[i]->Transform()->GetRelativeScale();
			if (vCurPos.y <= treePos.y)
			{
				treePos.z = vCurPos.z + 1.f;//뒤에 보이게
				breakObj[i]->Transform()->SetRelativePos(treePos);
			}
			else if (vCurPos.y > treePos.y)
			{
				treePos.z = vCurPos.z - 1.f;//앞에보이게
				breakObj[i]->Transform()->SetRelativePos(treePos);
			}
		}
	}
	Transform()->SetRelativePos(vCurPos);

	if (hp <= 0)
	{
		s_die->Play(1, 0.4f, false);
		Animator2D()->Play(L"GunnerKDown", false);
		hp == 0;
		if (hp == 0)
		{
			//distortion??
		}
	}
}


void CPlayerScript::Shoot()
{
	CGameObject* bullet = SpawnPrefab(L"texture\\Prefab\\playerbullet.prefab", Transform()->GetRelativePos());
	bullet->AddComponent(new CMissileScript);
	/*wstring strFolderpath = CPathMgr::GetInst()->GetContentPath();
	wstring relativepath = L"texture\\Prefab\\playerbullet.prefab";
	strFolderpath += relativepath;
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, strFolderpath.c_str(), L"rb");
	int ind = 0;
	fread(&ind, sizeof(int), 1, pFile);
	CGameObject* newPrefab = CLevelSaveLoad::LoadGameObject(pFile);
	Vec3 prefpos = newPrefab->Transform()->GetRelativePos() + Vec3(0.f,200.f,0.f);
	CMissileScript* script = newPrefab->GetScript<CMissileScript>();
	script->SetBulletDamage(GetPAttack());
	
	SpawnGameObject(newPrefab, prefpos, ind);*/
	//fclose(pFile);
}

void CPlayerScript::BeginOverlap(CCollider2D* _Other)
{
	if (L"rock" == _Other->GetOwner()->GetName())//GT ROck
	{
		ishit = true;
		if (_Other->GetOwner()->GetScript<CGTStoneScript>() != nullptr)
		{
			CGTStoneScript* stoneScript = _Other->GetOwner()->GetScript<CGTStoneScript>();
			int stoneDamage = stoneScript->SetDamageToPlayer();
			int count = GetDigitCount(stoneDamage);
			ShowDamage(stoneDamage, count, GetOwner());
			hp -= stoneDamage;
		}
		else
		{
			CGTStoneScript2* stoneScript2 = _Other->GetOwner()->GetScript<CGTStoneScript2>();
			int stoneDamage = stoneScript2->SetDamageToPlayer();
			int count = GetDigitCount(stoneDamage);
			ShowDamage(stoneDamage, count, GetOwner());
			hp -= stoneDamage;
		}
		DestroyObject(_Other->GetOwner());
	}
	else if (9 == _Other->GetOwner()->GetLayerIndex())//벽
	{
		iswall = true;
	}
	else if (L"tau" == _Other->GetOwner()->GetName())
	{
		CTauScript* tauScript = _Other->GetOwner()->GetScript<CTauScript>();
		int tauDamage = tauScript->SetDamageToPlayer();
		int count = GetDigitCount(tauDamage);
		isRush = true;
		ShowDamage(tauDamage, count, GetOwner());
		hp -= tauDamage;
		Animator2D()->Play(L"GunnerHit", false);
		Animator2D()->GetCurAnim()->Reset();
	}
	
}

void CPlayerScript::OnOverlap(CCollider2D* _Other)
{
	if (L"goblin" == _Other->GetOwner()->GetName())
	{
		CGoblinScript* goblinScript = _Other->GetOwner()->GetScript<CGoblinScript>();
		int goblinDamage = goblinScript->SetDamageToPlayer();
		int count = GetDigitCount(goblinDamage);
		goblinattackTerm += DT;
		if (goblinattackTerm == 0.1f )
		{
			ishit = true;
			ShowDamage(goblinDamage, count, GetOwner());
			hp -= goblinDamage;
		}
		else if (0.6f<goblinattackTerm && ishit == false)
		{
			ishit = true;
			ShowDamage(goblinDamage, count, GetOwner());
			hp -= goblinDamage;
			goblinattackTerm = 0.f;
		}
	}
	else if (L"BAtt" == _Other->GetOwner()->GetName())
	{
		CBakalAttackScript* BakalScript = _Other->GetOwner()->GetScript<CBakalAttackScript>();
		
		BakalattackTerm += DT;
		if (0.1f<BakalattackTerm&& BakalattackTerm< 0.11f)
		{
			ishit = true;
			int BakalDamage = BakalScript->SetDamageToPlayer() * 2;
			int count = GetDigitCount(BakalDamage);
			ShowDamage(BakalDamage, count, GetOwner());
			hp -= BakalDamage;
		}
		else if (0.8f < BakalattackTerm && BakalattackTerm < 0.81f && ishit == false)
		{
			ishit = true;
			int BakalDamage = BakalScript->SetDamageToPlayer() * 2;
			int count = GetDigitCount(BakalDamage);
			ShowDamage(BakalDamage, count, GetOwner());
			hp -= BakalDamage;
		}
		else if (1.5f  < BakalattackTerm && ishit == false)
		{
			ishit = true;
			int BakalDamage = BakalScript->SetDamageToPlayer() * 2;
			int count = GetDigitCount(BakalDamage);
			ShowDamage(BakalDamage, count, GetOwner());
			hp -= BakalDamage;
		}
	}
	else if (L"Bakal" == _Other->GetOwner()->GetName())
	{
		CBakalScript* BakalScript = _Other->GetOwner()->GetScript<CBakalScript>();
		BakalstampTerm += DT;
		if (rest == false)
		{
			if (2.f < BakalstampTerm && BakalstampTerm < 2.6f)
			{
				int BakalDamage = BakalScript->SetDamageToPlayer() * 5;
				int count = GetDigitCount(BakalDamage);
				ShowDamage(BakalDamage, count, GetOwner());
				hp -= BakalDamage;
				rest = true; 
			}
			
		}
		if (rest)
		{
			if (4.f < BakalstampTerm)
			{
				rest = false;
				BakalstampTerm = 0.f;
			}
		}
		
	}
	else if (L"coin" == _Other->GetOwner()->GetName())
	{
		isItem = true;
		if (KEY_TAP(KEY::X) )
		{
			Animator2D()->Play(L"GunnerStandup", false);
			Animator2D()->GetCurAnim()->Reset();
			CItemScript* itemscript = _Other->GetOwner()->GetScript<CItemScript>();
			coin += itemscript->GiveCoinPrice();
			DestroyObject(_Other->GetOwner());
			isItem = false;
		}
	}
	else if (L"hpP" == _Other->GetOwner()->GetName())
	{
		isItem = true;
		if (KEY_TAP(KEY::X))
		{
			Animator2D()->Play(L"GunnerStandup", false);
			Animator2D()->GetCurAnim()->Reset();
			CItemScript* itemscript = _Other->GetOwner()->GetScript<CItemScript>();
			hpPotion += itemscript->ItemCount();
			DestroyObject(_Other->GetOwner());
			isItem = false;
		}
	}
	else if (L"mpP" == _Other->GetOwner()->GetName())
	{
		isItem = true;
		if (KEY_TAP(KEY::X))
		{
			Animator2D()->Play(L"GunnerStandup", false);
			Animator2D()->GetCurAnim()->Reset();
			CItemScript* itemscript = _Other->GetOwner()->GetScript<CItemScript>();
			mpPotion += itemscript->ItemCount();
			DestroyObject(_Other->GetOwner());
			isItem = false;
		}
	}
}

void CPlayerScript::EndOverlap(CCollider2D* _Other)
{
	if (L"tau" == _Other->GetOwner()->GetName())
	{
		isRush = false;
	}
	else if (L"Coin" == _Other->GetOwner()->GetName())
	{
		isItem = false;
	}
	else if (L"BAtt" == _Other->GetOwner()->GetName())
	{
		BakalattackTerm = 0.f;
	}
}

void CPlayerScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_fSpeed, sizeof(float), 1, _File);
	//hp등 저장
}

void CPlayerScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_fSpeed, sizeof(float), 1, _File);
}

void CPlayerScript::RX78()
{
	mp -= 50;
	Vec3 playerpos = Transform()->GetRelativePos();
	CGameObject* prefab = SpawnPrefab(L"texture\\Prefab\\RX78.prefab", playerpos + Vec3(-20.f, -50.f, -100.f));
	prefab->AddComponent(new CRX78Script);
	CRX78Script* pScript = prefab->GetScript<CRX78Script>();
	pScript->SetHP(maxhp);
	pScript->SetDefense(defense);
	pScript->SetLevel(level);
	pScript->SetDamage(MAttack);
}

void CPlayerScript::EZ8()
{
	mp -= 72;
	// 습득 레벨 2/4/6
	// 시전시간 0.3
	// 쿨타임 5.5
	// MP소모량 72 /79 /86
	//캐릭터 HP 300%
	//캐릭터 방어력 300%
	//지속시간 7초
	//마법 공격력 4613% / 5039 / 5465
	//폭팔 범위 132%
	Vec3 playerpos = Transform()->GetRelativePos();
	CGameObject* prefab = SpawnPrefab(L"texture\\Prefab\\EZ8.prefab", playerpos + Vec3(-20.f,-50.f,-100.f));
	prefab->AddComponent(new CEZ8Script);
	CEZ8Script* pScript = prefab->GetScript<CEZ8Script>();
	pScript->SetHP(maxhp);
	pScript->SetDefense(defense);
	pScript->SetLevel(level);
	pScript->SetDamage(MAttack);
}

void CPlayerScript::EXS()
{
	mp -= 85;
	Vec3 playerpos = Transform()->GetRelativePos();
	CGameObject* prefab = SpawnPrefab(L"texture\\Prefab\\EXS.prefab", playerpos + Vec3(-20.f, -50.f, -100.f));
	prefab->AddComponent(new CEXSScript);
	CEXSScript* pScript = prefab->GetScript<CEXSScript>();
	pScript->SetHP(maxhp);
	pScript->SetDefense(defense);
	pScript->SetLevel(level);
	pScript->SetDamage(MAttack);
}

void CPlayerScript::DRunner()
{
	// 습득 레벨 4/6/8
	// 시전시간 0.5
	// 쿨타임 34
	// MP소모량 638 /726 /814
	//미사일 공격력 3825/4214/4601/
	// 미사일 18발
	//자폭 공격력 12150/13384/14616%/
	// 자폭 탐색 범위 550
	//폭팔 범위 100%
	mp -= 150;
	Vec3 playerpos = Transform()->GetRelativePos();
	CGameObject* prefab = SpawnPrefab(L"texture\\Prefab\\DRunner.prefab", Vec3(playerpos.x,playerpos.y, 20.f));
	prefab->AddComponent(new CDRunnerScript);
	CDRunnerScript* pScript = prefab->GetScript<CDRunnerScript>();
	pScript->SetLevel(level);
	pScript->SetDamage(MAttack);
}

void CPlayerScript::FastKnee()
{
	mp -= 30;
	//마하킥
	// 습득 레벨 1/3/5
	// 시전시간 = 즉시시전
	// 쿨타임 1.8
	//물리 공격력 110/120/130%
	Animator2D()->Play(L"GunnerFastKnee", false);
	Animator2D()->GetCurAnim()->Reset();
	CGameObject* FastKnee = new CGameObject;
	FastKnee->SetName(L"FastKnee");
	FastKnee->AddComponent(new CTransform);
	FastKnee->AddComponent(new CCollider2D);
	FastKnee->Transform()->SetAbsolute(false);
	FastKnee->Collider2D()->SetOffsetScale(Vec2(150.f, 100.f));
	if(m_Dir)
		FastKnee->Collider2D()->SetOffsetPos(Vec2(50.f, -40.f));
	else
		FastKnee->Collider2D()->SetOffsetPos(Vec2(-50.f, -40.f));
	SpawnGameObject(FastKnee, GetOwner()->Transform()->GetRelativePos(), L"PlayerProjectile");
	FastKnee->SetLifeSpan(0.3f); 
}

void CPlayerScript::Kneekick()
{
	mp -= 30;
	Animator2D()->Play(L"GunnerKneeKick", false);
	Animator2D()->GetCurAnim()->Reset();
	CGameObject* KneeKick = new CGameObject;
	KneeKick->SetName(L"KneeKick");
	KneeKick->AddComponent(new CTransform);
	KneeKick->AddComponent(new CCollider2D);
	KneeKick->Transform()->SetAbsolute(false);
	KneeKick->Collider2D()->SetOffsetScale(Vec2(50.f, 130.f));
	if (m_Dir)
		KneeKick->Collider2D()->SetOffsetPos(Vec2(20.f, -15.f));
	else
		KneeKick->Collider2D()->SetOffsetPos(Vec2(-20.f, -15.f));
	SpawnGameObject(KneeKick, GetOwner()->Transform()->GetRelativePos(), L"PlayerProjectile");
	KneeKick->SetLifeSpan(0.3f);
}

void CPlayerScript::Windmil()
{
	mp -= 18;
	// 습득 레벨 1/3/5
	// 시전시간 = 즉시시전
	// 쿨타임 4
	// MP소모량 = 18/20/23
	// 타격수 = 2
	//물리 공격력 1125/1239/1354
	Animator2D()->Play(L"GunnerWindmil", false);
	Animator2D()->GetCurAnim()->Reset();
	CGameObject* Windmil = new CGameObject;
	Windmil->SetName(L"Windmil");
	Windmil->AddComponent(new CTransform);
	Windmil->AddComponent(new CCollider2D);
	Windmil->Transform()->SetAbsolute(false);
	Windmil->Collider2D()->SetOffsetScale(Vec2(170.f, 150.f));
	if (m_Dir)
		Windmil->Collider2D()->SetOffsetPos(Vec2(0.f, -15.f));
	else
		Windmil->Collider2D()->SetOffsetPos(Vec2(-0.f, -15.f));
	Windmil->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	SpawnGameObject(Windmil, GetOwner()->Transform()->GetRelativePos(), L"PlayerProjectile");
	Windmil->SetLifeSpan(0.3f);
	Vec3 playerpos = Transform()->GetRelativePos();
	playerpos -= Vec3(0.f, 15.f, 0.f);
	SpawnPrefab(L"texture\\Prefab\\WindmilEffect.prefab", playerpos, 0.3f);
}

void CPlayerScript::ShowLevel(int _level)
{
	Vec3 playerpos = Transform()->GetRelativePos();
	CGameObject* prefab = SpawnPrefab(L"texture\\Prefab\\LV.prefab", playerpos + Vec3(-20.f, 50.f, 0.f));
	if (level == 1)
	{
		CGameObject* prefab = SpawnPrefab(L"texture\\Prefab\\Level1.prefab", playerpos + Vec3(0.f, 50.f, 0.f));
	}
	else if (level == 2)
	{
		CGameObject* prefab = SpawnPrefab(L"texture\\Prefab\\Level2.prefab", playerpos + Vec3(0.f, 50.f, 0.f));
	}
	else	if (level == 3)
	{
		CGameObject* prefab = SpawnPrefab(L"texture\\Prefab\\Level3.prefab", playerpos + Vec3(0.f, 50.f, 0.f));
	}
	else	if (level == 4)
	{
		CGameObject* prefab = SpawnPrefab(L"texture\\Prefab\\Level4.prefab", playerpos + Vec3(0.f, 50.f, 0.f));
	}
	else	if (level == 5)
	{
		CGameObject* prefab = SpawnPrefab(L"texture\\Prefab\\Level1.prefab", playerpos + Vec3(0.f, 50.f, 0.f));
	}
	else if (level == 6)
	{
		CGameObject* prefab = SpawnPrefab(L"texture\\Prefab\\Level1.prefab", playerpos + Vec3(0.f, 50.f, 0.f));
	}
	else	if (level == 7)
	{
		CGameObject* prefab = SpawnPrefab(L"texture\\Prefab\\Level1.prefab", playerpos + Vec3(0.f, 50.f, 0.f));
	}
	else	if (level == 8)
	{
		CGameObject* prefab = SpawnPrefab(L"texture\\Prefab\\Level1.prefab", playerpos + Vec3(0.f, 50.f, 0.f));
	}
	else	if (level == 9)
	{
		CGameObject* prefab = SpawnPrefab(L"texture\\Prefab\\Level1.prefab", playerpos + Vec3(0.f, 50.f, 0.f));
	}
	else	if (level == 10)
	{
		CGameObject* prefab = SpawnPrefab(L"texture\\Prefab\\Level1.prefab", playerpos + Vec3(0.f, 50.f, 0.f));
	}
		
	/*prefab->AddComponent(new CRX78Script);
	CRX78Script* pScript = prefab->GetScript<CRX78Script>();*/
}