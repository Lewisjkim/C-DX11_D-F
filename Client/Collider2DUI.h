#pragma once
#include "ComponentUI.h"
class Collider2DUI :
    public ComponentUI
{
private:
    int shape = 0;
    virtual int render_update() override;
public:
    Collider2DUI();
    ~Collider2DUI();
};

