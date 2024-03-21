#pragma once
#include "CRes.h"
#include<DirectXTex\DirectXTex.h>
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex//DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectXTex//DirectXTex.lib")
#endif
// ���� ��뵵 �ϰ� �ҷ��ͼ� ��뵵 �ؾߵȴ�.
// �پ��� ���˵��� �ҷ��� ����� �� �־�� �ȴ�.
// ����: system memory�� �ҷ��� ->GPU�� view�� ���ε��ؼ� �Ѱ��ش�

class CTexture :
    public CRes
{
private:
    ComPtr<ID3D11Texture2D>             m_Tex2D;
    
    // Device�ʿ� ���� ���� �ߴµ� �ؽ��� ������ ���� ���ʿ��� �ϱ⿡ ����� �ű�
    ComPtr<ID3D11ShaderResourceView>    m_SRV;// Texture binding�� view
    ComPtr<ID3D11RenderTargetView>      m_RTV;
    ComPtr<ID3D11DepthStencilView>      m_DSV;
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;

    D3D11_TEXTURE2D_DESC                m_Desc;
    ScratchImage                        m_Image;// ��� Ȯ���� �̹����� �������� ��� - DirectXTex���� ����

    UINT                                m_iRecentNum;

public:
    int Create(UINT _Width, UINT _Height, DXGI_FORMAT _pixelformat
        , UINT _BindFlag, D3D11_USAGE _Usage);

    int Create(ComPtr<ID3D11Texture2D> _tex2D);//������� �ؽ��� ���

public:
    float Width() { return (float)m_Desc.Width; }
    float Height() { return (float)m_Desc.Height; }
    ComPtr<ID3D11Texture2D>             GetTex2D() { return m_Tex2D; }
    ComPtr<ID3D11ShaderResourceView>    GetSRV() { return m_SRV; }
    ComPtr<ID3D11RenderTargetView>	    GetRTV() { return m_RTV; }
    ComPtr<ID3D11DepthStencilView>	    GetDSV() { return m_DSV; }
    ComPtr<ID3D11UnorderedAccessView>   GetUAV() { return m_UAV; }

private:
    virtual int Load(const wstring& _strFilePath) override; //���� ��ο��� �ҷ�����

public:
    virtual int Save(const wstring& _strRelativePath) override;// ����η� ����

public:
    // _PipelineStage : PIPELINE_STAGE
    void UpdateData(int _iRegisterNum, int _PipelineStage); //texture
    void UpdateData_CS(int _iRegisterNum, bool _bShaderRes); //noisetex, colorchange shader
    void Clear();
    static void Clear(int _iRegisterNum);
    void Clear_CS(bool _bShaderRes);
private: 
    virtual void UpdateData() override;

public:
    CTexture(bool _bEngine = false);
    ~CTexture();
};

