#pragma once
#include "CComponent.h"
#include "CAnim2D.h"
#include "ptr.h"
#include "CTexture.h"

class CAnim2D;

class CAnimator2D :
    public CComponent
{
private:
    map<wstring, CAnim2D*>      m_mapAnim;  // Animation 목록
    CAnim2D*                                  m_pCurAnim; // 현재 재생중인 Animation
    bool                                             m_bRepeat;  // 반복

    wstring animfilepath;

public:
    virtual void finaltick() override;
    void UpdateData();
    void Clear();

public:
    void Play(const wstring& _strName, bool _bRepeat);
    CAnim2D* FindAnim(const wstring& _strName);
    void DelAnim(const wstring& _strName);  
    CAnim2D* GetCurAnim() { return m_pCurAnim; }
    bool GetRepeat() { return m_bRepeat; }
    CAnim2D* LoadAnimationfromfile();
    CAnim2D* LoadAnimation(const wstring& _strRelativePath);
    map<wstring, CAnim2D*> GetAnimMap() { return m_mapAnim; }
    void CreateAnimation(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vBackSize, int _FrameCount, int _FPS, Vec2 _Offset);
    
public:
    virtual void SaveToLevelFile(FILE* _File)override;
    virtual void LoadFromLevelFile(FILE* _File)override;

    CLONE(CAnimator2D);
public:
    CAnimator2D();
    ~CAnimator2D();
};

