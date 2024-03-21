#pragma once
#include "C:\DirectX_Practice\DirectX_\External\Include\Engine\CScript.h"
class CBakalStampScript :
    public CScript
{
private:
    int                  m_Damage;
    float               m_time;
    Vec3              m_Pos;
    Vec3              m_Rot;
public:
    virtual void begin() override;
    virtual void tick() override;
    CLONE(CBakalStampScript)
public:
    CBakalStampScript();
    ~CBakalStampScript();
};

