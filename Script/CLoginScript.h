#pragma once
#include <Engine/CScript.h>
class CLoginScript:
    public CScript
{
private:
public:
    virtual void begin() override;
    virtual void tick() override;

public:
    virtual void BeginOverlap(CCollider2D* _Other) override;

    CLONE(CLoginScript)
public:
    CLoginScript();
    ~CLoginScript();
};
