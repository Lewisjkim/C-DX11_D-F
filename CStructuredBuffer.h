#pragma once
#include "CEntity.h"

class CStructuredBuffer :
    public CEntity
{
private:
    ComPtr<ID3D11Buffer>                m_SB; //register binding main buffer
    ComPtr<ID3D11ShaderResourceView>    m_SRV;//read only
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;//read write

    ComPtr<ID3D11Buffer>                m_SB_CPU_Read; // GPU->sys
    ComPtr<ID3D11Buffer>                m_SB_CPU_Write;// sys->Gpu

    D3D11_BUFFER_DESC                   m_tDesc;

    UINT                                m_iElementSize;
    UINT                                m_iElementCount;

    SB_TYPE                             m_Type;
    bool                                m_bSysAccess; //시스템 메모리쪽 접근 사용할 것이냐?

    UINT                                m_iRecentRegisterNum;//최근에 바인딩한 register num

public:
    void Create(UINT _iElementSize, UINT _iElementCount, SB_TYPE _Type, bool _bSysAccess, void* _pSysMem = nullptr);
    void SetData(void* _pSrc, UINT _iSize = 0);
    void GetData(void* _pDst);

    // PIPELINE_STAGE
    void UpdateData(UINT _iRegisterNum, UINT _iPipeLineStage);
    void UpdateData_CS(UINT _iRegisterNum, bool _IsShaderRes);

    void Clear();
    void Clear_CS(bool _IsShaderRes);

    UINT GetElementSize() { return m_iElementSize; }
    UINT GetElementCount() { return m_iElementCount; }
    UINT GetBufferSize() { return m_iElementSize * m_iElementCount; }


    CLONE_DISABLE(CStructuredBuffer);

public:
    CStructuredBuffer();
    ~CStructuredBuffer();
};

