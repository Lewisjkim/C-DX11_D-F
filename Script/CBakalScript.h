#pragma once
#include "C:\DirectX_Practice\DirectX_\External\Include\Engine\CScript.h"
class CBakalScript :
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
    bool           isRight;
    bool           iswall;
    Vec3           BakalPos;
    float           timer;
    int              random;
    bool           randomItem;
    float           monsterType;
    float           randomTime = 0;
    int              minus;
    bool           movetype;
    bool           readytoAttack;
    bool           isattackRight;
    bool           isattackLeft;
    bool           isHit;
    bool           isoverlap;
    bool           rest;
    bool           isKnockback;
    bool           isKneeKick;
    float          kneekickTime;
    float           attackTerm;
    CGameObject* rightattack;
    CGameObject* leftattack;
    CGameObject* pDust;
    CGameObject* pMeteo;
    CGameObject* pMeteo2;
    CGameObject* pMeteo3;
public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void OnOverlap(CCollider2D* _Other) override;
    virtual void EndOverlap(CCollider2D* _Other) override;
    void GetDamageFromPlayer(int Damage) { m_Hp -= Damage; }//플레이어가 고블린에게 데미지 줄때
    int SetDamageToPlayer() { return m_Damage; }
    int GetMonsterHP() { return m_Hp; }
    int GetMonsterMaxHP() { return m_MaxHp; }

    CLONE(CBakalScript);
public:
    CBakalScript();
    ~CBakalScript();
};

