#pragma once
#include "C:\DirectX_Practice\DirectX_\External\Include\Engine\CScript.h"
class CGTStoneScript2 :
    public CScript
{
private:
    int                  m_Damage;
    float               m_time;
    float               m_Speed;
    Vec3              dirVector;
    Vec3              stonePos;
    bool               onemore;
public:
    virtual void begin() override;
    virtual void tick() override;
    virtual void BeginOverlap(CCollider2D* _Other) override;
    virtual void OnOverlap(CCollider2D* _Other) override;
    virtual void EndOverlap(CCollider2D* _Other) override;
    int SetDamageToPlayer() { return m_Damage; }
    CLONE(CGTStoneScript2);
public:
    CGTStoneScript2();
    ~CGTStoneScript2();
};

