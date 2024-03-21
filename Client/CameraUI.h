#pragma once
#include "ComponentUI.h"
class CameraUI :
    public ComponentUI
{
private:
    
    int m_index;
    bool check[MAX_LAYER] = {};
    bool checkall = false;
    bool uncheckall = false;
private:
    virtual int render_update() override;
public:
    CameraUI();
    ~CameraUI();
};

