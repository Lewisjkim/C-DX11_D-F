#pragma once
#include "ComponentUI.h"

#include <Engine\CScript.h>

class ScriptUI :
    public ComponentUI
{
private:
    CScript* m_pTargetScript; 
    string      m_strScriptName; // 스크립트 이름을 기억

public:
    virtual int render_update() override; // 이름 출력

public:
    void SetScript(CScript* _Script);

public:
    ScriptUI();
    ~ScriptUI();
};

