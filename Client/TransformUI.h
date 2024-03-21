#pragma once
#include "ComponentUI.h"

class CGameObject;

class TransformUI :
    public ComponentUI
{
private:
    bool absolute = false;
public:
    virtual int render_update() override;

public:
    TransformUI();
    ~TransformUI();
};

