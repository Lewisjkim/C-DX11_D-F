#include "pch.h"
#include "CLevelMgr.h"
#include "CGameObject.h"

#include "CLevel.h"
#include "CLayer.h"



CLevelMgr::CLevelMgr() :
	m_pCurLevel(nullptr)
{

}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_pCurLevel)
		delete m_pCurLevel;
}

void CLevelMgr::init()
{
	m_pCurLevel = new CLevel;
	m_pCurLevel->ChangeState(LEVEL_STATE::STOP);
}

void CLevelMgr::tick()
{
	m_pCurLevel->clear(); // 현재 레벨에 있는 오브젝트를 지워준다

	if (LEVEL_STATE::PLAY == m_pCurLevel->GetState()) // Play 상태일 때만 tick 
	{
		m_pCurLevel->tick();
	}
	m_pCurLevel->finaltick();
}

CGameObject* CLevelMgr::FindObjectByName(const wstring& _strName)
{
	return m_pCurLevel->FindObjectByName(_strName);
}

void CLevelMgr::FindObjectByName(const wstring& _strName, vector<CGameObject*>& _vec)
{
	m_pCurLevel->FindObjectByName(_strName, _vec);
}

void CLevelMgr::ChangeLevel(CLevel* _NextLevel)
{
	if (nullptr != m_pCurLevel)
	{
		delete m_pCurLevel;
		m_pCurLevel = nullptr;
	}

	m_pCurLevel = _NextLevel;
}

void CLevelMgr::SavePlayerInfo(int _level, int _hp, int _hpmax, int _mp, int _mpmax, int _exp, int _coin, int _hppotion, int _mppotion, int _fatigue)
{
	vecplayerInfo.clear();
	curLevel =			_level; 
	curHP =				_hp; 
	curMaxHP =		_hpmax; 
	curMP =			_mp;
	curMaxMP =	_mpmax; 
	curExp =			_exp; 
	curCoin =			_coin;
	curHPotion =	_hppotion; 
	curMPotion =	_mppotion;
	curFatigue =	_fatigue;
	vecplayerInfo.push_back(curLevel);
	vecplayerInfo.push_back(curHP);
	vecplayerInfo.push_back(curMaxHP);
	vecplayerInfo.push_back(curMP);
	vecplayerInfo.push_back(curMaxMP);
	vecplayerInfo.push_back(curExp);
	vecplayerInfo.push_back(curCoin);
	vecplayerInfo.push_back(curHPotion);
	vecplayerInfo.push_back(curMPotion);
	vecplayerInfo.push_back(curFatigue);
}

