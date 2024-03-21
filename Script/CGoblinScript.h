#pragma once
#include <Engine/CScript.h>
class CGoblinScript :
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
    Vec3           goblinPos;
    float           timer;
    int              random;
    float           monsterType;
    float           randomTime = 0;
    int              minus;
    bool           movetype;
    bool           readytoAttack;
    bool           isHit;
    bool           isKnockback;
    bool           isKneeKick;
    float          kneekickTime;
    float          attackTerm;
    bool          randomItem;
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
    CLONE(CGoblinScript);
public:
    CGoblinScript();
    ~CGoblinScript();
};


