#pragma once
#include <Engine/CScript.h>
#include <Engine/CGameObject.h>


class CPlayerScript :
    public CScript
{
public:
    //플레이어 정보==========
    int                hp;
    int                maxhp;
    int                mp;
    int                maxmp;
    int                defense;
    int                exp;
    int                maxexp;
    int                fatigue;
    int                level;
    int                PAttack;
    int                MAttack;
    float            m_fSpeed;
    bool            m_Dir = true;
    float            gravityAccel; //중력 가속도
    int                coin;
    int                hpPotion;
    int                mpPotion;
    CGameObject* GameID;
   // const wchar_t     gameID[256] = {};
    //행동 상태==============
    bool            readytorun;
    bool            iswalking;
    bool            isrunning;
    bool            ishit;
    bool            isknockdown;
    bool            isjump;
    bool            iswall;

    float            knockdownCount;
    float			m_time;
    float            ishitCount;
    float            StartTime;
    float            slidetime;
    float            ComboStart;
    float            ComboMax;
    float            jumpStartTime;
    float            jumpEndTime;
    float            jumppointY;//점프를 시작하는 현재 y값
    float            jumpmaxpointY;//점프 최대 y값
    float            standupCount;
    float            fastkneecount;
    float            kneekickcount;
    float            windmilcount;
    float            goblinattackTerm;
    float            BakalattackTerm;
    float            BakalstampTerm;
    float            tauattackTerm;
    bool            isRush;
    bool            isItem;
    bool            rest;
    int               comboCount;
    UINT      Target;
public:
    virtual void begin() override;
    virtual void tick() override; // transform에서 getpos후 setpos
    
    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void OnOverlap(CCollider2D* _Other) override;
    virtual void EndOverlap(CCollider2D* _Other) override;
   
private:
    void Shoot();
    //skills
    void RX78();
    void EZ8(); 
    void EXS();
    void DRunner();
    void FastKnee();
    void Kneekick();
    void Windmil();
    void ShowLevel(int _level);

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;
    bool GetDir() { return m_Dir; }
    int GetPlayerHP() { return hp; }
    int GetPlayerMaxHP() { return maxhp; }
    int GetPlayerMP() { return mp; }
    int GetPlayerMaxMP() { return maxmp; }
    int GetPlayerEXP() { return exp; }
    int GetCoin() { return coin; }
    int GetHPT() { return hpPotion; }
    int GetMPT() { return mpPotion; }
    int GetPAttack() { return PAttack; }
    int GetMAttack() { return MAttack; }
    int GetLevel() { return level; }
    void GetExpFromMonster(int _exp) { exp += _exp; }
    void GetDamageFromMonster(int _damage) { hp -= _damage; }
    void SetComboCount() { comboCount++; }
    void ComboReset() { comboCount = 0; };
    int GetComboCount() { return comboCount; }
    int GetFatigue() { return fatigue; }
    void SetTarget(UINT _TargetID) { Target = _TargetID; }
    UINT GetTarget() {return Target; }
    CLONE(CPlayerScript)
public:
    CPlayerScript();
    ~CPlayerScript();
};

