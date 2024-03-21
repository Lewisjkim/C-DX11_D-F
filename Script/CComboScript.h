#pragma once
#include "C:\DirectX_Practice\DirectX_\External\Include\Engine\CScript.h"
class CComboScript :
    public CScript
{
private:

public:
    virtual void begin() override;
    virtual void tick() override;
    CGameObject* mainCam;
    CGameObject* ComboText;
    float m_time;
    float m_resettime;
    CLONE(CComboScript);
public:
    CComboScript();
    ~CComboScript();
};

