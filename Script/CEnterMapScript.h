#pragma once
#include "C:\DirectX_Practice\DirectX_\External\Include\Engine\CScript.h"
class CEnterMapScript :
    public CScript
{
private:
    float m_time;
public:
    virtual void tick() override;
    CLONE(CEnterMapScript);
public:
    CEnterMapScript();
    ~CEnterMapScript();
};

