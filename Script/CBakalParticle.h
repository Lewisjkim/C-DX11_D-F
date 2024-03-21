#pragma once
#include "C:\DirectX_Practice\DirectX_\External\Include\Engine\CScript.h"
class CBakalParticle :
    public CScript
{
private:
    Vec3 BakalPos;
public:
    virtual void begin() override;
    virtual void tick() override;
    CLONE(CBakalParticle);
public:
    CBakalParticle();
    ~CBakalParticle();
};

