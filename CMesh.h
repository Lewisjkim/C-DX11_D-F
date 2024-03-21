#pragma once
#include "CRes.h"

//정점들을 관리(형태)

class CMesh :
    public CRes
{
private:
    ComPtr<ID3D11Buffer>    m_VB;
    D3D11_BUFFER_DESC       m_tVBDesc;
    UINT                    m_VtxCount;

    ComPtr<ID3D11Buffer>    m_IB;
    D3D11_BUFFER_DESC       m_tIBDesc;
    UINT                    m_IdxCount;

    void*                   m_pVtxSys; // 시스템 메모리 관리
    void*                   m_pIdxSys;

public:
    // 정점 시작주소, 몇개, 인덱스 시작주소, 몇개
    void Create(void* _VtxSysMem, UINT _iVtxCount, void* _IdxSysMem, UINT _IdxCount);

private:
    virtual int Load(const wstring& _strFilePath) { return S_OK; }
public:
    virtual int Save(const wstring& _strRelativePath) { return S_OK; }

    void render();
    void render_particle(UINT _iParticleCount);//파티클 렌더

private:
    virtual void UpdateData() override; // 리소스 바인딩 - 데이터 갱신

public:
    CMesh(bool _bEngine = false);
    ~CMesh();
};

