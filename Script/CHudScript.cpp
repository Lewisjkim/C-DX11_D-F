#include "pch.h"
#include "CHudScript.h"
#include "CPlayerScript.h"
#include "CGoblinScript.h"
#include "CGoblinThrowerScript.h"
#include "CTauScript.h"
#include "CBakalScript.h"
#include <Engine/CEventMgr.h>
#include "/DirectX_Practice/DirectX_/Project/Client/CLevelSaveLoad.h"

CHudScript::CHudScript()		:
	CScript((UINT)SCRIPT_TYPE::HUDSCRIPT)
{
}

CHudScript::~CHudScript()
{
}

void CHudScript::begin()
{
	startpos = Transform()->GetRelativePos();
	fadeCount = 0;
	fadeAmount = 0;
}

void CHudScript::tick()
{
	fadeCount += DT;
	vector<CGameObject*> vecplayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
	vector<CGameObject*> vecmonster = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(3)->GetParentObject();
	CPlayerScript* plScript = vecplayer[0]->GetScript<CPlayerScript>();
	/*CPlayerScript* plScript = vecmonster[0]->GetScript<CPlayerScript>();
	for (size_t i = 0; i < vecmonster.size(); i++)
	{

	}*/

	if (GetOwner()->GetName() == L"hpcirlce")
	{
		float hp = plScript->GetPlayerHP();
		float maxhp = plScript->GetPlayerMaxHP();
		float hpratio = hp / maxhp;
		Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();
		pMtrl->SetScalarParam(FLOAT_0, &hp);
		pMtrl->SetScalarParam(FLOAT_1, &maxhp);
		
		Vec3 scale = Transform()->GetRelativeScale();
		
		float vertical;
		vertical = startpos.y - scale.y * (1 - hpratio);
		Transform()->SetRelativePos(startpos.x,vertical, startpos.z);
	}
	else if (GetOwner()->GetName() == L"mpcircle")
	{
		float mp = plScript->GetPlayerMP();
		float maxmp = plScript->GetPlayerMaxMP();
		float mpratio = mp / maxmp;
		Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();
		pMtrl->SetScalarParam(FLOAT_0, &mp);
		pMtrl->SetScalarParam(FLOAT_1, &maxmp);

		Vec3 scale = Transform()->GetRelativeScale();

		float vertical;
		vertical = startpos.y - scale.y * (1 - mpratio);
		Transform()->SetRelativePos(startpos.x, vertical, startpos.z);
	}//
	else if (GetOwner()->GetName() == L"ExpBar")
	{
		float exp = plScript->GetPlayerEXP();
		Vec3 barPos = Transform()->GetRelativePos();
		Vec3 barscale = Transform()->GetRelativeScale();
		float scaleratio = 970.f * (exp / 100.f);
		Transform()->SetRelativeScale(Vec3(scaleratio,barscale.y,barscale.z));
		barPos.x = 1 - (970.f - scaleratio)/2;
		Transform()->SetRelativePos(barPos);
	}
	else if (GetOwner()->GetName() == L"FGbar")
	{
		float fatigue = plScript->GetFatigue();
		Vec3 barPos = Transform()->GetRelativePos();
		Vec3 barscale = Transform()->GetRelativeScale();
		float scaleratio =240.f * (fatigue / 10.f);
		Transform()->SetRelativeScale(Vec3(scaleratio, barscale.y, barscale.z));
		barPos.x = 24 - (240.f - scaleratio) / 2;
		Transform()->SetRelativePos(barPos);
	}
	else if (GetOwner()->GetName() == L"JPicon")
	{
		if (KEY_TAP(KEY::Z))
		{
			Ptr<CMaterial> coolmaterial = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\JackSpike_c.mtrl");
			MeshRender()->SetMaterial(coolmaterial);
		}
		else if (KEY_RELEASE(KEY::Z))
		{
			Ptr<CMaterial> coolmaterial = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\JackSpike.mtrl");
			MeshRender()->SetMaterial(coolmaterial);
		}
	}
	else if (GetOwner()->GetName() == L"RX78icon")
	{
		if (KEY_TAP(KEY::A))
		{
			Ptr<CMaterial> coolmaterial = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\RX78_c.mtrl");
			MeshRender()->SetMaterial(coolmaterial);
		}
		else if (KEY_RELEASE(KEY::A))
		{
			Ptr<CMaterial> coolmaterial = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\RX78.mtrl");
			MeshRender()->SetMaterial(coolmaterial);
		}
	}
	else if (GetOwner()->GetName() == L"EZ8icon")
	{
		if (KEY_TAP(KEY::S))
		{
			Ptr<CMaterial> coolmaterial = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\EZ8_c.mtrl");
			MeshRender()->SetMaterial(coolmaterial);
		}
		else if (KEY_RELEASE(KEY::S))
		{
			Ptr<CMaterial> coolmaterial = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\EZ8.mtrl");
			MeshRender()->SetMaterial(coolmaterial);
		}
	}
	else if (GetOwner()->GetName() == L"EXSicon")
	{
		if (KEY_TAP(KEY::D))
		{
			Ptr<CMaterial> coolmaterial = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\EXS_c.mtrl");
			MeshRender()->SetMaterial(coolmaterial);
		}
		else if (KEY_RELEASE(KEY::D))
		{
			Ptr<CMaterial> coolmaterial = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\EXS.mtrl");
			MeshRender()->SetMaterial(coolmaterial);
		}
	}
	else if (GetOwner()->GetName() == L"DRicon")
	{
		if (KEY_TAP(KEY::R))
		{
			Ptr<CMaterial> coolmaterial = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\DRunner_c.mtrl");
			MeshRender()->SetMaterial(coolmaterial);
		}
		else if (KEY_RELEASE(KEY::R))
		{
			Ptr<CMaterial> coolmaterial = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\DRunner.mtrl");
			MeshRender()->SetMaterial(coolmaterial);
		}
	}
	else if (GetOwner()->GetName() == L"WMicon")
	{
		if (KEY_TAP(KEY::E))
		{
			Ptr<CMaterial> coolmaterial = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Windmil_c.mtrl");
			MeshRender()->SetMaterial(coolmaterial);
		}
		else if (KEY_RELEASE(KEY::E))
		{
			Ptr<CMaterial> coolmaterial = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\Windmil.mtrl");
			MeshRender()->SetMaterial(coolmaterial);
		}
	}
	else if (GetOwner()->GetName() == L"MKicon")
	{
		if (KEY_TAP(KEY::F))
		{
			Ptr<CMaterial> coolmaterial = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\MahaKick_c.mtrl");
			MeshRender()->SetMaterial(coolmaterial);
		}
		else if (KEY_RELEASE(KEY::F))
		{
			Ptr<CMaterial> coolmaterial = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\MahaKick.mtrl");
			MeshRender()->SetMaterial(coolmaterial);
		}
	}
	else if (GetOwner()->GetName() == L"hppnum")
	{
		int amount = plScript->GetHPT();
		int digitcount = GetDigitCount(amount);
		Vec3 textpos = Transform()->GetRelativePos();
		while (amount != 0 && amount < 10)
		{
			digitcount--;
			int digit = pow(10, digitcount);
			int num = amount / digit;// 100/3
			textpos += Vec3(30.f, 0.f, 0.f);
			textpos.z = 0.f;
			if (num == 1)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level1.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 2)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level2.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 3)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level3.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 4)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level4.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 5)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level5.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 6)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level6.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 7)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level7.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 8)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level8.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 9)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level9.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 0)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level0.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}

			amount = amount % digit;// 100%3 알
			if (digit > 0 && 0 == amount)
			{
				int num = GetDigitCount(digit);
				num -= 1;
				for (size_t i = 0; i < num; i++)
				{
					textpos += Vec3(30.f, 0.f, 0.f);
					Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level0.mtrl");
					MeshRender()->SetMaterial(nlevel);
				}
			}
		}
	}
	else if (GetOwner()->GetName() == L"mppnum")
	{
		int amount = plScript->GetMPT();
		int digitcount = GetDigitCount(amount);
		Vec3 textpos = Transform()->GetRelativePos();
		while (amount != 0 && amount < 10)
		{
			digitcount--;
			int digit = pow(10, digitcount);
			int num = amount / digit;// 100/3
			textpos += Vec3(30.f, 0.f, 0.f);
			textpos.z = 0.f;
			if (num == 1)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level1.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 2)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level2.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 3)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level3.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 4)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level4.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 5)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level5.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 6)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level6.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 7)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level7.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 8)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level8.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 9)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level9.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 0)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level0.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}

			amount = amount % digit;// 100%3 알
			if (digit > 0 && 0 == amount)
			{
				int num = GetDigitCount(digit);
				num -= 1;
				for (size_t i = 0; i < num; i++)
				{
					textpos += Vec3(30.f, 0.f, 0.f);
					Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level0.mtrl");
					MeshRender()->SetMaterial(nlevel);
				}
			}
		}
	}
	else if (GetOwner()->GetName() == L"lvnum")
	{
		int playerlevel = plScript->GetLevel();
		int digitcount = GetDigitCount(playerlevel);
		Vec3 textpos = Transform()->GetRelativePos();
		while (playerlevel != 0&& playerlevel<10)
		{
			digitcount--;
			int digit = pow(10, digitcount);
			int num = playerlevel / digit;// 100/3
			textpos += Vec3(30.f, 0.f, 0.f);
			textpos.z = 0.f;
			if (num == 1)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level1.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 2)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level2.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 3)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level3.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 4)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level4.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 5)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level5.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 6)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level6.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 7)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level7.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 8)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level8.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 9)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level9.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}
			else if (num == 0)
			{
				Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level0.mtrl");
				MeshRender()->SetMaterial(nlevel);
			}

			playerlevel = playerlevel % digit;// 100%3 알
			if (digit > 0 && 0 == playerlevel)
			{
				int num = GetDigitCount(digit);
				num -= 1;
				for (size_t i = 0; i < num; i++)
				{
					textpos += Vec3(30.f, 0.f, 0.f);
					Ptr<CMaterial> nlevel = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\level0.mtrl");
					MeshRender()->SetMaterial(nlevel);
				}
			}
		}
	}
	else if (GetOwner()->GetName() == L"monface")
	{
		for (size_t i = 0; i < vecmonster.size(); i++)
		{
			if (vecmonster[i]->GetID() == plScript->GetTarget())
			{
				if (L"goblin" == vecmonster[i]->GetName())
				{
					Ptr<CMaterial> mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\GoblinIcon.mtrl");
					MeshRender()->SetMaterial(mtrl);
					Transform()->SetRelativeScale(Vec3(24.f, 24.f, 0.f));
				}
				else if (L"goblinT" == vecmonster[i]->GetName())
				{
					Ptr<CMaterial> mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\GoblinThrowerIcon.mtrl");
					MeshRender()->SetMaterial(mtrl);
					Transform()->SetRelativeScale(Vec3(24.f, 24.f, 0.f));
				}
				else if (L"tau" == vecmonster[i]->GetName())
				{
					Ptr<CMaterial> mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\TauIcon.mtrl");
					MeshRender()->SetMaterial(mtrl);
					Transform()->SetRelativeScale(Vec3(24.f, 24.f, 0.f));
				}
				else if (L"Bakal" == vecmonster[i]->GetName())
				{
					Ptr<CMaterial> mtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\BakalIcon.mtrl");
					MeshRender()->SetMaterial(mtrl);
					Transform()->SetRelativeScale(Vec3(24.f, 24.f, 0.f));
				}
			}
		}
	}
	else if (GetOwner()->GetName() == L"monhpbar")
	{
		Vec3 barPos = Transform()->GetRelativePos();
		Vec3 barscale = Transform()->GetRelativeScale();
		for (size_t i = 0; i < vecmonster.size(); i++)
		{
			if (vecmonster[i]->GetID() == plScript->GetTarget())
			{
				if (L"goblin" == vecmonster[i]->GetName())
				{
					CGoblinScript* monscript = vecmonster[i]->GetScript <CGoblinScript>();
					float hp = monscript->GetMonsterHP();
					float maxhp = monscript->GetMonsterMaxHP();
					float hpratio = hp / maxhp;
					float changesize = 597.f * hpratio;
					Transform()->SetRelativeScale(Vec3(changesize, barscale.y, barscale.z));
					barPos.x = -263 - (597.f - changesize) / 2;
					Transform()->SetRelativePos(barPos);
				}
				else if (L"goblinT" == vecmonster[i]->GetName())
				{
					CGoblinThrowerScript* monscript = vecmonster[i]->GetScript <CGoblinThrowerScript>();
					float hp = monscript->GetMonsterHP();
					float maxhp = monscript->GetMonsterMaxHP();
					float hpratio = hp / maxhp;
					float changesize = 597.f * hpratio;
					Transform()->SetRelativeScale(Vec3(changesize, barscale.y, barscale.z));
					barPos.x = -263 - (597.f - changesize) / 2;
					Transform()->SetRelativePos(barPos);
				}
				else if (L"tau" == vecmonster[i]->GetName())
				{
					CTauScript* monscript = vecmonster[i]->GetScript <CTauScript>();
					float hp = monscript->GetMonsterHP();
					float maxhp = monscript->GetMonsterMaxHP();
					float hpratio = hp / maxhp;
					float changesize = 597.f * hpratio;
					Transform()->SetRelativeScale(Vec3(changesize, barscale.y, barscale.z));
					barPos.x = -263 - (597.f - changesize) / 2;
					Transform()->SetRelativePos(barPos);
				}
				else if (L"Bakal" == vecmonster[i]->GetName())
				{
					CBakalScript* monscript = vecmonster[i]->GetScript <CBakalScript>();
					float hp = monscript->GetMonsterHP();
					float maxhp = monscript->GetMonsterMaxHP();
					float hpratio = hp / maxhp;
					float changesize = 597.f * hpratio;
					Transform()->SetRelativeScale(Vec3(changesize, barscale.y, barscale.z));
					barPos.x = -263 - (597.f - changesize) / 2;
					Transform()->SetRelativePos(barPos);
				}
			}
		}
	}
	else if (GetOwner()->GetName() == L"fade")
	{
		if(fadeAmount<0.8f&&fadeCount<1.f)
			fadeAmount = 0.5f* fadeCount;
		
		Ptr<CMaterial> pMtrl = MeshRender()->GetMaterial();
		pMtrl->SetScalarParam(FLOAT_0, &fadeAmount);
		
	}
	else if (GetOwner()->GetName() == L"goback")
	{
		if (KEY_PRESSED(KEY::ENTER))
		{
			Ptr<CMaterial> pressed = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\BackToVillage2.mtrl");
			MeshRender()->SetMaterial(pressed);
		}
		if (KEY_RELEASE(KEY::ENTER))
		{
			Ptr<CMaterial> released = CResMgr::GetInst()->FindRes<CMaterial>(L"material\\BackToVillage3.mtrl");
			MeshRender()->SetMaterial(released);

			vector<CGameObject*> vecPlayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
			CPlayerScript* pScript = vecPlayer[0]->GetScript<CPlayerScript>();
			//플레이어 현재 정보를 가져와서 가지고있게한다.
			lv = pScript->GetLevel();
			hp = pScript->GetPlayerHP();
			hpmax = pScript->GetPlayerMaxHP();
			mp = pScript->GetPlayerMP();
			mpmax = pScript->GetPlayerMaxMP();
			exp = pScript->GetPlayerEXP();
			coin = pScript->GetCoin();
			hpt = pScript->GetHPT();
			mpt = pScript->GetMPT();
			fatigue = pScript->GetFatigue();
			CLevelMgr::GetInst()->SavePlayerInfo(lv, hp, hpmax, mp, mpmax, exp, coin, hpt, mpt, fatigue);

			CLevel* pNewLevel = CLevelSaveLoad::Stop(L"Level\\Village.lv", LEVEL_STATE::STOP);
			pNewLevel->SetName(L"Village");

			tEvent evn = {};
			evn.Type = EVENT_TYPE::LEVEL_CHANGE;
			evn.wParam = (DWORD_PTR)pNewLevel;
			CEventMgr::GetInst()->AddEvent(evn);

			Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Map\\mirkwood_boss.ogg");
			pSound->Stop();
			Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Map\\amb_forest_16_bird.ogg");
			pSound2->Play(0, 0.5f, false);
		}
	}
}

void CHudScript::BeginOverlap(CCollider2D* _Other)
{
}

void CHudScript::OnOverlap(CCollider2D* _Other)
{
}

void CHudScript::EndOverlap(CCollider2D* _Other)
{
}
