#pragma once
#include "CSingleton.h"

class CLevel;
class CGameObject;

class CLevelMgr :
    public CSingleton<CLevelMgr>
{
    SINGLE(CLevelMgr);

private:
    CLevel* m_pCurLevel;//���� �����Ǿ��ִ� ����
    vector<int> vecplayerInfo;
    int curLevel;
    int curHP;
    int curMaxHP;
    int curMP;
    int curMaxMP;
    int curExp;
    int curCoin;
    int curHPotion;
    int curMPotion;
    int curFatigue;
public:
    CLevel* GetCurLevel() { return m_pCurLevel; }
    CGameObject* FindObjectByName(const wstring& _strName);
    void FindObjectByName(const wstring& _strName, vector<CGameObject*>& _Out);

    void ChangeLevel(CLevel* _NextLevel);
    void SavePlayerInfo(int _level, int _hp, int _hpmax, int _mp, int _mpmax, int _exp, int _coin, int _hppotion, int _mppotion, int _fatigue);
    vector<int> GetPlayerInfo() { return vecplayerInfo; }

public:
    void init(); //������Ʈ�� ���⼭ �����Ѵ�
    void tick();
};

