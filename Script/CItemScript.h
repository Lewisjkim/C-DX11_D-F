#pragma once
#include "C:\DirectX_Practice\DirectX_\External\Include\Engine\CScript.h"
class CItemScript :
    public CScript
{
private:
    int     coinPrice;
    int     itemCount;
public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void OnOverlap(CCollider2D* _Other) override;
    virtual void EndOverlap(CCollider2D* _Other) override;
    int GiveCoinPrice() { return coinPrice; }
    int ItemCount() { return itemCount; }
    CLONE(CItemScript);
public:
    CItemScript();
    ~CItemScript();
};

