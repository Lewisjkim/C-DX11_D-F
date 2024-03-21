#pragma once
#include "C:\DirectX_Practice\DirectX_\External\Include\Engine\CScript.h"
class CLoadingCirlce :
    public CScript
{
private:
    float m_time;
public:
    virtual void tick() override;
    CLONE(CLoadingCirlce);
    CLoadingCirlce();
    ~CLoadingCirlce();
};

