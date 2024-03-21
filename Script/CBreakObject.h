#pragma once
#include "C:\DirectX_Practice\DirectX_\External\Include\Engine\CScript.h"
class CBreakObject :
    public CScript
{
private:

public:
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;
    void ShowBObjectParticle(wstring relativpath, Vec3 _vPos);
private:
    CLONE(CBreakObject);
public:
    CBreakObject();
    ~CBreakObject();
};

