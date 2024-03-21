#pragma once
#include "CEntity.h"
#include "ptr.h"
#include "CTexture.h"

class CAnimator2D;

class CAnim2D :
    public CEntity
{
private:
    CAnimator2D*                m_pOwner;
    vector<tAnim2DFrm>    m_vecFrm;
    Vec2                                  m_vBackSize;
    Ptr<CTexture>                 m_AtlasTex;
    wstring                             loadedfilepath;
    int                                     m_iCurFrm;
    float                                  m_fTime;
    bool                                 m_bFinish;
public:
    void finaltick();
    void Create(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _BackSize, int _FrameCount, int _FPS, Vec2 _Offset);

    void SetAtlasTex(Ptr<CTexture> _Tex) { m_AtlasTex = _Tex; }
    void SetBackSize(Vec2 backsize) { m_vBackSize = backsize; }
    Vec2 GetBackSize() { return m_vBackSize; }
    const tAnim2DFrm& GetCurFrame() { return m_vecFrm[m_iCurFrm]; }
    vector<tAnim2DFrm>& GetFrame() { return m_vecFrm; }

    size_t GetFramSize() { return m_vecFrm.size(); }
    Ptr<CTexture> GetAtlasTex() { return m_AtlasTex; }
    wstring GetLoadedFilePath() { return loadedfilepath; }
    bool IsFinish() { return m_bFinish; }
    void Reset()
    {
        m_iCurFrm = 0;
        m_fTime = 0.f;
        m_bFinish = false;
    }

    void Save();
    void Load();
    void Load(const wstring& _strRelativePath);

public:
    virtual void SaveToLevelFile(FILE* _File);
    virtual void LoadFromLevelFile(FILE* _File);

    CLONE(CAnim2D);
public:
    CAnim2D();
    ~CAnim2D();

    friend class CAnimator2D;
};

