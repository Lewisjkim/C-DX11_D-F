#pragma once
#include "C:\DirectX_Practice\DirectX_\External\Include\Engine\CScript.h"
class CTauScript :
    public CScript
{
private:
    int              m_Damage;
    float           m_time;
    float           m_Speed;
    int              m_Hp;
    int              m_MaxHp;
    float           deadcount;
    int              m_exp;
    bool           isDead = true;
    bool           isLeft = true;
    bool           iswall;
    Vec3           TauPos;
    float           timer;
    int              random;
    bool           randomItem;
    float           monsterType;
    float           randomTime = 0;
    int              minus;
    bool           movetype;
    bool           readytoAttack;
    bool           readytoRush;
    bool           isHit;
    bool           isKnockback;
    bool           isKneeKick;
    float          kneekickTime;
    float           attackTerm;
    float           chargetime;
    bool           setdir;
    float           rest;
    bool            isRush;
public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void OnOverlap(CCollider2D* _Other) override;
    virtual void EndOverlap(CCollider2D* _Other) override;
    void GetDamageFromPlayer(int Damage) { m_Hp -= Damage; }//1인 데미지 줄때
    int SetDamageToPlayer() { return m_Damage; }
    int GetMonsterHP() { return m_Hp; }
    int GetMonsterMaxHP() { return m_MaxHp; }

    CLONE(CTauScript);
public:
    CTauScript();
    ~CTauScript();
};

