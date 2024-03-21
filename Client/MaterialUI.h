#pragma once
#include "ResUI.h"
class MaterialUI :
    public ResUI
{
private:
    TEX_PARAM       m_eSelected;//선택된 파라미터를 기억하는 변수

public:
    virtual int render_update() override;

public:
    void SelectTexture(DWORD_PTR _Key);

public:
    MaterialUI();
    ~MaterialUI();
};

