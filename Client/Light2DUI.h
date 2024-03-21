#pragma once
#include "ComponentUI.h"
#include <Engine/CLight2D.h>

class Light2DUI :
    public ComponentUI
{
private:
    int m_lighttype;
    Vec4 m_lightDir;
    float f_lightDir[4] = { m_lightDir.x, m_lightDir.y, m_lightDir.z, m_lightDir.w };
    Vec4 m_lightDif;
   
    Vec4 m_lightAmb;
    float f_lightAmb[4] = { m_lightAmb.x, m_lightAmb.y, m_lightAmb.z, m_lightAmb.w };
    float m_lightRadius;
    float m_lightAngle;
private:
    virtual int render_update() override;
public:
    Light2DUI();
    ~Light2DUI();
};

