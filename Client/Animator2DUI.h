#pragma once
#include "ComponentUI.h"
#include <Engine/CTexture.h>
#include <Engine/CResMgr.h>

class CAnim2D;

class Animator2DUI :
    public ComponentUI
{

private:
    CAnim2D* pAnim;
    Ptr<CTexture> m_Atlas;
    Ptr<CTexture> LoadTexture;
    char filename[128] = "Animation Name";
    string animName;
    int FrameCount;
    wstring wanimName;
    wstring animKey;
    wstring wFilePath;
    float Duration;
    bool Repeat;
    float anim_w;
    float anim_h;
    Vec2 LT;
    float fLT[2] = { LT.x, LT.y };
    Vec2 RB;
    float fRB[2] = { RB.x, RB.y };
    Vec2 BackSize;
    float fBackSize[2] = { BackSize.x, BackSize.y };
    Vec2 Offset;
    float fOffset[2] = { Offset.x, Offset.y };
    wchar_t* loadedfilepath = {};
   vector<tAnim2DFrm> vecFrame;
public:
    wstring GetAnimName() { return wanimName; }//****
    int GetFrameCount() { return FrameCount; }//****
    bool GetRepeat() { return Repeat; }//****

    virtual int render_update() override;
public:
    Animator2DUI();
    ~Animator2DUI();
};

