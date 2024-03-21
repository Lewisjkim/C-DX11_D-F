#include "pch.h"
#include "CDoorScript.h"
#include "CPlayerScript.h"
#include <Engine/CGameObject.h>
#include <Engine/CPathMgr.h>
#include <Engine/CEventMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLevelMgr.h>
#include "/DirectX_Practice/DirectX_/Project/Client/CLevelSaveLoad.h"

CDoorScript::CDoorScript()	:
	CScript((UINT)SCRIPT_TYPE::DOORSCRIPT),
	lv(0),
	hp(0),
	hpmax(0),
	mp(0),
	mpmax(0),
	exp(0),
	coin(0),
	hpt(0),
	mpt(0),
	fatigue(0)
{
}

CDoorScript::~CDoorScript()
{
}

void CDoorScript::begin()
{
	Vec3 DoorPos = Transform()->GetRelativePos();
	wstring name = GetOwner()->GetName();
	if(CLevelMgr::GetInst()->GetCurLevel()->GetName() != L"SeriaRoom")//fense생성
	{
		if (GetOwner()->GetName() == L"LDoor")
		{
			wstring strFolderpath = CPathMgr::GetInst()->GetContentPath();
			wstring relativepath = L"texture\\Prefab\\leftfense.prefab";
			strFolderpath += relativepath;
			FILE* pFile = nullptr;
			errno_t iErrNum = _wfopen_s(&pFile, strFolderpath.c_str(), L"rb");
			int ind = 0;
			fread(&ind, sizeof(int), 1, pFile);
			LFense = CLevelSaveLoad::LoadGameObject(pFile);
			LFense->SetName(L"LFense");
			Vec3 prefpos = DoorPos + Vec3(5.f, -10.f, 0.f);
			SpawnGameObject(LFense, prefpos, ind);

			fclose(pFile);
		}
		else if (GetOwner()->GetName() == L"RDoor")
		{
			wstring strFolderpath = CPathMgr::GetInst()->GetContentPath();
			wstring relativepath = L"texture\\Prefab\\rightfense.prefab";
			strFolderpath += relativepath;
			FILE* pFile = nullptr;
			errno_t iErrNum = _wfopen_s(&pFile, strFolderpath.c_str(), L"rb");
			int ind = 0;
			fread(&ind, sizeof(int), 1, pFile);
			RFense = CLevelSaveLoad::LoadGameObject(pFile);
			RFense->SetName(L"RFense");
			Vec3 prefpos = DoorPos + Vec3(-5.f, -10.f, 0.f);
			SpawnGameObject(RFense, prefpos, ind);

			fclose(pFile);
		}
	}
}

void CDoorScript::tick()
{
	vector<CGameObject*> vecMonster = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(3)->GetParentObject();
	if (CLevelMgr::GetInst()->GetCurLevel()->GetName() != L"SeriaRoom"||	CLevelMgr::GetInst()->GetCurLevel()->GetName() != L"Village"
		/*|| CLevelMgr::GetInst()->GetCurLevel()->GetName() != L"PracticeRoom"*/)//세리아 룸 or 마을 or 연습방이 아니면 몬스터가 있을 때 철창삭제
	{
		vector<CGameObject*> vecObj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(0)->GetParentObject();
		for (size_t i = 0; i < vecObj.size(); i++)
		{
			if (vecMonster.size() == 0)
			{
				if (vecObj[i]->GetName() == L"LFense" || vecObj[i]->GetName() == L"RFense")
				{
					DestroyObject(vecObj[i]);
				}
			}
		}
	}
	else
	{
		vector<CGameObject*> vecObj = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(0)->GetParentObject();
		for (size_t i = 0; i < vecObj.size(); i++)
		{
			if (vecObj[i]->GetName() == L"LFense" || vecObj[i]->GetName() == L"RFense")
			{
				DestroyObject(vecObj[i]);
			}
		}
	}
}

void CDoorScript::BeginOverlap(CCollider2D* _Other)
{
	if (2 == _Other->GetOwner()->GetLayerIndex())//player와 충돌하면
	{
		vector<CGameObject*> vecPlayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2)->GetParentObject();
		CPlayerScript* pScript =vecPlayer[0]->GetScript<CPlayerScript>();
		vector<CGameObject*> vecMonster = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(3)->GetParentObject();
		if (vecMonster.size() == 0)
		{
		
			wstring curLevelName = CLevelMgr::GetInst()->GetCurLevel()->GetName();
			if (GetOwner()->GetName() == L"LDoor")//왼쪽문=======================================
			{
				if (curLevelName == L"Village")
				{
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

					CLevel* pNewLevel = CLevelSaveLoad::Stop(L"Level\\PracticeRoom.lv", LEVEL_STATE::STOP);
					pNewLevel->SetName(L"PracticeRoom");

					tEvent evn = {};
					evn.Type = EVENT_TYPE::LEVEL_CHANGE;
					evn.wParam = (DWORD_PTR)pNewLevel;
					CEventMgr::GetInst()->AddEvent(evn);

					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Map\\amb_forest_16_bird.ogg");
					pSound->Stop();
					Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Map\\elven_guard.ogg");
					pSound2->Play(0, 0.5f, false);
				}
				else if (curLevelName == L"PracticeRoom")
				{
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

					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Map\\elven_guard.ogg");
					pSound->Stop();
					Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Map\\amb_forest_16_bird.ogg");
					pSound2->Play(0, 0.5f, false);
				}
				else if (curLevelName == L"stage1_2")//stage1->practiceroom
				{
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

					CLevel* pNewLevel = CLevelSaveLoad::Stop(L"Level\\stage1_1.lv", LEVEL_STATE::STOP);
					pNewLevel->SetName(L"stage1_1");

					tEvent evn = {};
					evn.Type = EVENT_TYPE::LEVEL_CHANGE;
					evn.wParam = (DWORD_PTR)pNewLevel;
					CEventMgr::GetInst()->AddEvent(evn);

					//Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Map\\mirkwood.ogg");
					//pSound->Stop();
					//Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Map\\elven_guard.ogg");
					//pSound2->Play(0, 0.5f, false);

				}
				else if (curLevelName == L"stage1_3")//stage1->practiceroom
				{
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

					CLevel* pNewLevel = CLevelSaveLoad::Stop(L"Level\\stage1_2.lv", LEVEL_STATE::STOP);
					pNewLevel->SetName(L"stage1_2");

					tEvent evn = {};
					evn.Type = EVENT_TYPE::LEVEL_CHANGE;
					evn.wParam = (DWORD_PTR)pNewLevel;
					CEventMgr::GetInst()->AddEvent(evn);

					//Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Map\\mirkwood.ogg");
					//pSound->Stop();
					//Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Map\\elven_guard.ogg");
					//pSound2->Play(0, 0.5f, false);

				}
				else if (curLevelName == L"stage1_Boss")//stage1->practiceroom
				{
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

					CLevel* pNewLevel = CLevelSaveLoad::Stop(L"Level\\stage1_3.lv", LEVEL_STATE::STOP);
					pNewLevel->SetName(L"stage1_3");

					tEvent evn = {};
					evn.Type = EVENT_TYPE::LEVEL_CHANGE;
					evn.wParam = (DWORD_PTR)pNewLevel;
					CEventMgr::GetInst()->AddEvent(evn);
					
					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\monster\\bakal_dragon_king_final_02.ogg");
					pSound->Stop();
					Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Map\\mirkwood.ogg");
					pSound2->Play(0, 0.5f, false);

				}
			}
			else if (GetOwner()->GetName() == L"RDoor")//오른쪽 문=======================
			{
				if (curLevelName == L"SeriaRoom")
				{
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

					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Map\\seria_gate.ogg");
					pSound->Stop();
					Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Map\\amb_forest_16_bird.ogg");
					pSound2->Play(0, 0.5f, false);
					/*CLevel* NewLevel = NextLevel(L"Level\\PracticeRoom.lv");
					NewLevel->SetName(L"PracticeRoom");
					tEvent evn = {};
					evn.Type = EVENT_TYPE::LEVEL_CHANGE;
					evn.wParam = (DWORD_PTR)NewLevel;

					CEventMgr::GetInst()->AddEvent(evn);

					CLevel* curLevel = CLevelMgr::GetInst()->GetCurLevel();
					curLevel->ChangeState(LEVEL_STATE::STOP);
					CLevelSaveLoad::Play(L"Level\\SeriaRoom.lv", curLevel);*/
				}
				else if (curLevelName == L"Village")
				{
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

					CLevel* pNewLevel = CLevelSaveLoad::Stop(L"Level\\SeriaRoom.lv", LEVEL_STATE::STOP);
					pNewLevel->SetName(L"SeriaRoom");

					tEvent evn = {};
					evn.Type = EVENT_TYPE::LEVEL_CHANGE;
					evn.wParam = (DWORD_PTR)pNewLevel;
					CEventMgr::GetInst()->AddEvent(evn);

					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Map\\amb_forest_16_bird.ogg");
					pSound->Stop();
					Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Map\\seria_gate.ogg");
					pSound2->Play(0, 0.5f, false);
				}
				else if (curLevelName == L"PracticeRoom")
				{
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

					CLevel* pNewLevel = CLevelSaveLoad::Stop(L"Level\\EnterMap.lv", LEVEL_STATE::STOP);
					pNewLevel->SetName(L"EnterMap");

					tEvent evn = {};
					evn.Type = EVENT_TYPE::LEVEL_CHANGE;
					evn.wParam = (DWORD_PTR)pNewLevel;
					CEventMgr::GetInst()->AddEvent(evn);

					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Map\\elven_guard.ogg");
					pSound->Stop();
					Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Map\\mirkwood.ogg");
					pSound2->Play(0, 0.5f, false);
				}
				else if (curLevelName == L"stage1_1")
				{
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

					CLevel* pNewLevel = CLevelSaveLoad::Stop(L"Level\\stage1_2.lv", LEVEL_STATE::STOP);
					pNewLevel->SetName(L"stage1_2");

					tEvent evn = {};
					evn.Type = EVENT_TYPE::LEVEL_CHANGE;
					evn.wParam = (DWORD_PTR)pNewLevel;
					CEventMgr::GetInst()->AddEvent(evn);

					/*Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Map\\elven_guard.ogg");
					pSound->Stop();
					Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Map\\mirkwood.ogg");
					pSound2->Play(0, 0.5f, false);*/
				}
				else if (curLevelName == L"stage1_2")
				{
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

					CLevel* pNewLevel = CLevelSaveLoad::Stop(L"Level\\stage1_3.lv", LEVEL_STATE::STOP);
					pNewLevel->SetName(L"stage1_3");

					tEvent evn = {};
					evn.Type = EVENT_TYPE::LEVEL_CHANGE;
					evn.wParam = (DWORD_PTR)pNewLevel;
					CEventMgr::GetInst()->AddEvent(evn);

					/*Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Map\\elven_guard.ogg");
					pSound->Stop();
					Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Map\\mirkwood.ogg");
					pSound2->Play(0, 0.5f, false);*/
				}
				else if (curLevelName == L"stage1_3")
				{
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

					CLevel* pNewLevel = CLevelSaveLoad::Stop(L"Level\\stage1_Boss.lv", LEVEL_STATE::STOP);
					pNewLevel->SetName(L"stage1_Boss");

					tEvent evn = {};
					evn.Type = EVENT_TYPE::LEVEL_CHANGE;
					evn.wParam = (DWORD_PTR)pNewLevel;
					CEventMgr::GetInst()->AddEvent(evn);

					Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Map\\mirkwood.ogg");
					pSound->Stop();
					Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\monster\\bakal_dragon_king_final_02.ogg");
					pSound2->Play(0, 0.5f, false);
				}
			}
		}
	}
}

CLevel* CDoorScript::NextLevel(wstring path)
{
	wstring strFolderpath = CPathMgr::GetInst()->GetContentPath();
	strFolderpath += path;
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, strFolderpath.c_str(), L"rb");

	if (nullptr == pFile)
		return nullptr;

	CLevel* NewLevel = new CLevel;

	// 레벨 이름
	wstring strLevelName;
	LoadWString(strLevelName, pFile);
	NewLevel->SetName(strLevelName);

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = NewLevel->GetLayer(i);

		// 레이어 이름
		wstring LayerName;
		LoadWString(LayerName, pFile);
		pLayer->SetName(LayerName);

		// 게임 오브젝트 개수
		size_t objCount = 0;
		fread(&objCount, sizeof(size_t), 1, pFile);

		// 각 게임오브젝트
		for (size_t j = 0; j < objCount; ++j)
		{
			CGameObject* pNewObj = CLevelSaveLoad::LoadGameObject(pFile);
			NewLevel->AddGameObject(pNewObj, i, false);
		}
	}

	fclose(pFile);

	NewLevel->ChangeState(LEVEL_STATE::STOP);
	return NewLevel;
}

