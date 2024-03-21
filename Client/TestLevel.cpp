#include "pch.h"
#include "TestLevel.h"
#include <Engine/CFontMgr.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>
#include <Engine\CEventMgr.h>

#include <Engine\CResMgr.h>
#include <Engine\CCollisionMgr.h>

#include <Script\CPlayerScript.h>
#include <Script\CGoblinScript.h>
#include <Script\CMissileScript.h>
#include <Engine/CPrefab.h>
#include "CLevelSaveLoad.h"

void CreateTestLevel()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->ChangeState(LEVEL_STATE::STOP);

	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"Map");
	pCurLevel->GetLayer(2)->SetName(L"Player");
	pCurLevel->GetLayer(3)->SetName(L"Monster");
	pCurLevel->GetLayer(4)->SetName(L"PlayerProjectile");
	pCurLevel->GetLayer(5)->SetName(L"MonsterProjectile");
	pCurLevel->GetLayer(6)->SetName(L"NPC");
	pCurLevel->GetLayer(7)->SetName(L"Door");
	pCurLevel->GetLayer(8)->SetName(L"HitObject");
	pCurLevel->GetLayer(9)->SetName(L"Boundary");
	pCurLevel->GetLayer(10)->SetName(L"MainCam");
	pCurLevel->GetLayer(11)->SetName(L"UICam");
	pCurLevel->GetLayer(31)->SetName(L"ViewPortUI");

	CLevel* pNewLevel = CLevelSaveLoad::Stop(L"Level\\PracticeRoom.lv", LEVEL_STATE::STOP);
	pNewLevel->SetName(L"PracticeRoom");
	
	tEvent evn = {};
	evn.Type = EVENT_TYPE::LEVEL_CHANGE;
	evn.wParam = (DWORD_PTR)pNewLevel;
	CEventMgr::GetInst()->AddEvent(evn);

	CLevelMgr::GetInst()->SavePlayerInfo(1, 1000, 1000, 500, 500, 0, 100, 2, 1, 10);
	Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\203680770.wem");
	pSound->Play(1, 0.5f, false);

	// 충돌 시킬 레이어 짝 지정
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster");
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Map");
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"MonsterProjectile");
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Boundary");
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Door");
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"HitObject");
	CCollisionMgr::GetInst()->LayerCheck(L"PlayerProjectile", L"HitObject");
	CCollisionMgr::GetInst()->LayerCheck(L"Monster", L"Map");
	CCollisionMgr::GetInst()->LayerCheck(L"Monster", L"PlayerProjectile");
	CCollisionMgr::GetInst()->LayerCheck(L"Monster", L"Boundary");
	CCollisionMgr::GetInst()->LayerCheck(L"Monster", L"HitObject");

	return;
	
}