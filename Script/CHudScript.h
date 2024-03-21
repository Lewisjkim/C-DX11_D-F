#pragma once
#include "C:\DirectX_Practice\DirectX_\External\Include\Engine\CScript.h"
class CHudScript :
    public CScript
{
private:
    Vec3 startpos;
    float fadeCount;
    float fadeAmount;

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
public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void OnOverlap(CCollider2D* _Other) override;
    virtual void EndOverlap(CCollider2D* _Other) override;

    CLONE(CHudScript);
public:
    CHudScript();
    ~CHudScript();
};

