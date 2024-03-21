#pragma once
#include "C:\DirectX_Practice\DirectX_\External\Include\Engine\CScript.h"
class CGameObject;
class CLevel;
class CDoorScript :
    public CScript
{
private:         
    int lv;
    int hp;
    int hpmax;
    int mp;
    int mpmax;
    int exp;
    int coin;
    int hpt;
    int mpt;
    int fatigue;
    CGameObject* LFense;
    CGameObject* RFense;
public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;
    CLevel* NextLevel(wstring path);
private:
    CLONE(CDoorScript);
public:
    CDoorScript();
    ~CDoorScript();
};

