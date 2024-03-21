#pragma once
#include "C:\DirectX_Practice\DirectX_\External\Include\Engine\CScript.h"
class CBakalAttackScript :
    public CScript
{
private:
    int                  m_Damage;
    float               m_time;
    Vec3              m_Pos;
    Vec3              BakalRot;
    Vec3              BakalPos;
public:
    virtual void begin() override;
    virtual void tick() override;
    int SetDamageToPlayer() { return m_Damage; }
    CLONE(CBakalAttackScript)
public:
    CBakalAttackScript();
    ~CBakalAttackScript();
};

