#pragma once
#include "C:\DirectX_Practice\DirectX_\External\Include\Engine\CScript.h"
class CDamageScript :
    public CScript
{
private:

public:
    virtual void begin() override;
    virtual void tick() override; // transform¿¡¼­ getposÈÄ setpos

    CLONE(CDamageScript);
public:
    CDamageScript();
    ~CDamageScript();
};

