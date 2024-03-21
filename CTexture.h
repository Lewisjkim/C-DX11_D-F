#pragma once
#include "CRes.h"
#include<DirectXTex\DirectXTex.h>
#ifdef _DEBUG
#pragma comment(lib, "DirectXTex//DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectXTex//DirectXTex.lib")
#endif
// 직접 사용도 하고 불러와서 사용도 해야된다.
// 다양한 포맷들을 불러와 사용할 수 있어야 된다.
// 구조: system memory에 불러와 ->GPU에 view로 바인딩해서 넘겨준다

class CTexture :
    public CRes
{
private:
    ComPtr<ID3D11Texture2D>             m_Tex2D;
    
    // Device쪽에 만들어서 관리 했는데 텍스쳐 생성을 이제 이쪽에서 하기에 여기로 옮김
    ComPtr<ID3D11ShaderResourceView>    m_SRV;// Texture binding용 view
    ComPtr<ID3D11RenderTargetView>      m_RTV;
    ComPtr<ID3D11DepthStencilView>      m_DSV;
    ComPtr<ID3D11UnorderedAccessView>   m_UAV;

    D3D11_TEXTURE2D_DESC                m_Desc;
    ScratchImage                        m_Image;// 모든 확장자 이미지를 가져오는 기능 - DirectXTex에서 제공

    UINT                                m_iRecentNum;

public:
    int Create(UINT _Width, UINT _Height, DXGI_FORMAT _pixelformat
        , UINT _BindFlag, D3D11_USAGE _Usage);

    int Create(ComPtr<ID3D11Texture2D> _tex2D);//만들어진 텍스쳐 사용

public:
    float Width() { return (float)m_Desc.Width; }
    float Height() { return (float)m_Desc.Height; }
    ComPtr<ID3D11Texture2D>             GetTex2D() { return m_Tex2D; }
    ComPtr<ID3D11ShaderResourceView>    GetSRV() { return m_SRV; }
    ComPtr<ID3D11RenderTargetView>	    GetRTV() { return m_RTV; }
    ComPtr<ID3D11DepthStencilView>	    GetDSV() { return m_DSV; }
    ComPtr<ID3D11UnorderedAccessView>   GetUAV() { return m_UAV; }

private:
    virtual int Load(const wstring& _strFilePath) override; //절대 경로에서 불러오기

public:
    virtual int Save(const wstring& _strRelativePath) override;// 상대경로로 저장

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

