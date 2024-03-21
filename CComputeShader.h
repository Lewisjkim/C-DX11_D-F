#pragma once
#include "CShader.h"

class CComputeShader :
    public CShader
{
private:
    ComPtr<ID3D11ComputeShader>     m_CS;
    ComPtr<ID3DBlob>                m_CSBlob;
    int gpt[3] = {};
protected:
    // ���̴��� ������ ��� ������
    tMtrlConst                      m_Const;

    // �׷� ����
    UINT                            m_iGroupX;
    UINT                            m_iGroupY;
    UINT                            m_iGroupZ;

    // �׷� 1���� ������ ����
    UINT                            m_iGroupPerThreadX;//�׷�1���� ������X ����
    UINT                            m_iGroupPerThreadY;
    UINT                            m_iGroupPerThreadZ;

public:
    void CreateComputeShader(const wstring& _strFileName, const string& _strFuncName);
    void Execute();//(�����ϴ�)

    UINT GetGPTX() { return  gpt[0] = m_iGroupPerThreadX; }
    UINT GetGPTY() { return  gpt[1] = m_iGroupPerThreadY; }
    UINT GetGPTZ() { return  gpt[2] = m_iGroupPerThreadZ; }

private:
    virtual void UpdateData() = 0;
    virtual void Clear() = 0;

    CLONE_DISABLE(CComputeShader);
public:
    CComputeShader();
    ~CComputeShader();
};

