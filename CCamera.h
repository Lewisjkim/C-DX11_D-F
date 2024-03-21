#pragma once
#include "CComponent.h"

class CCamera :
    public CComponent
{
private:
    float       m_fAspectRatio; // ��Ⱦ��
    float       m_fScale;       // Orthographic���� ����ϴ� ī�޶� ����

    PROJ_TYPE   m_ProjType;     // �������

    Matrix      m_matView;      // View ���
    Matrix      m_matProj;      // ���� ��� = 1. ����������� 2. �����������

    UINT        m_iLayerMask;   //��ġ �ʴ� ���̾ �������� �ʰ� ���ִ� ��ü

    int         m_iCamIdx;      // ī�޶� �켱����

    vector<CGameObject*>    m_vecOpaque;        // ������ ������Ʈ
    vector<CGameObject*>    m_vecMask;          // ������, ����
    vector<CGameObject*>    m_vecTransparent;   // ������
    vector<CGameObject*>    m_vecUI;            // UI
    vector<CGameObject*>    m_vecPost;          // �� ó��

    
public:
    //��Ⱦ��
    void SetAspectRatio(float _ratio) { m_fAspectRatio = _ratio; }
    float GetAspectRatio() { return m_fAspectRatio; }

    //ī�޶� �������
    void SetProjType(PROJ_TYPE _Type)   { m_ProjType = _Type; }
    PROJ_TYPE GetProjType()             { return m_ProjType; }

    //ī�޶� ����
    void SetScale(float _fScale)        { m_fScale = _fScale; }
    float GetScale()                    { return m_fScale; }

    //���̾� ����
    void SetLayerMask(int _iLayer, bool _Visible);
    void SetLayerMaskAll( bool _Visible);

    //ī�޶� �켱����
    void SetCameraIndex(int _idx);

    //�� ���, 
    const Matrix& GetViewMat()          { return m_matView; }
    const Matrix& GetProjMat()          { return m_matProj; }

    int GetCamIndex() { return m_iCamIdx; }

public:
    void SortObject(); // ������Ʈ �з�
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

