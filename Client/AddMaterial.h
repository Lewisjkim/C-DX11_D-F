#pragma once
#include "UI.h"
class AddMaterial :
	public UI
{
private:
    int        m_int = 0;
    float    m_float = 0.f;
    float    a_vec2[2] = { 0.f, 0.f };
    Vec2    m_Vec2 = Vec2(0.f, 0.f);
    float    a_vec4[4] = { 0.f, 0.f, 0.f, 0.f };
    Vec4    m_Vec4 = Vec4(0.f, 0.f, 0.f, 0.f);
    char m_name[128] = "Name";
    char s_name[128] = "Name";
    char t_name[128] = "Name";

    int iParam = 0;
    int fParam = 0;
    int Vec2Param = 0;
    int Vec4Param = 0;
public:
    virtual void init() override;
    virtual void tick() override;
    virtual int render_update() override;

public:
    AddMaterial();
    ~AddMaterial();
};

