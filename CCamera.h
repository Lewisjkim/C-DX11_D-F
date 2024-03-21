#pragma once
#include "CComponent.h"

class CCamera :
    public CComponent
{
private:
    float       m_fAspectRatio; // 종횡비
    float       m_fScale;       // Orthographic에서 사용하는 카메라 배율

    PROJ_TYPE   m_ProjType;     // 투영방식

    Matrix      m_matView;      // View 행렬
    Matrix      m_matProj;      // 투영 행렬 = 1. 직교투영방식 2. 원근투영방식

    UINT        m_iLayerMask;   //원치 않는 레이어를 렌더하지 않게 해주는 객체

    int         m_iCamIdx;      // 카메라 우선순위

    vector<CGameObject*>    m_vecOpaque;        // 불투명 오브젝트
    vector<CGameObject*>    m_vecMask;          // 불투명, 투명
    vector<CGameObject*>    m_vecTransparent;   // 반투명
    vector<CGameObject*>    m_vecUI;            // UI
    vector<CGameObject*>    m_vecPost;          // 후 처리

    
public:
    //종횡비
    void SetAspectRatio(float _ratio) { m_fAspectRatio = _ratio; }
    float GetAspectRatio() { return m_fAspectRatio; }

    //카메라 투영방식
    void SetProjType(PROJ_TYPE _Type)   { m_ProjType = _Type; }
    PROJ_TYPE GetProjType()             { return m_ProjType; }

    //카메라 배율
    void SetScale(float _fScale)        { m_fScale = _fScale; }
    float GetScale()                    { return m_fScale; }

    //레이어 제한
    void SetLayerMask(int _iLayer, bool _Visible);
    void SetLayerMaskAll( bool _Visible);

    //카메라 우선순위
    void SetCameraIndex(int _idx);

    //뷰 행렬, 
    const Matrix& GetViewMat()          { return m_matView; }
    const Matrix& GetProjMat()          { return m_matProj; }

    int GetCamIndex() { return m_iCamIdx; }

public:
    void SortObject(); // 오브젝트 분류
    void render();

public:
    virtual void begin() override;
    virtual void finaltick() override;

private:
    void clear();
    void render_opaque();
    void render_mask();
    void render_transparent();
    void render_postprocess();
    void render_ui();

    void CalcViewMat();
    void CalcProjMat();


    CLONE(CCamera);

public:
    virtual void SaveToLevelFile(FILE* _File)override;
    virtual void LoadFromLevelFile(FILE* _File)override;

public:
    CCamera();
    CCamera(const CCamera& _Other);
    ~CCamera();

};

