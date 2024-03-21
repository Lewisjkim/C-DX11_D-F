#include "pch.h"
#include "CDevice.h"

#include "CEngine.h"
#include "CConstBuffer.h"

#include "CResMgr.h"

CDevice::CDevice()  :
    m_hWnd(nullptr),
    m_ViewPort {},
    m_arrConstBuffer{}
{
}

CDevice::~CDevice()
{
    Safe_Del_Array(m_arrConstBuffer);
}

int CDevice::init(HWND _hwnd, UINT _iWidth, UINT _iHeight)
{
    //RenderTarget 해상도를 window 해상도와 같게한다(제일 화질이 좋아서)
    m_hWnd = _hwnd;
    m_vRenderResolution = Vec2((float)_iWidth, (float)_iHeight);

    GlobalData.Resolution = m_vRenderResolution;

    int iFlag = 0;
#ifdef _DEBUG
    iFlag = D3D11_CREATE_DEVICE_DEBUG; //debug시에만 디버그 레이어를 사용
#endif

    D3D_FEATURE_LEVEL eLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;


    //Device, Context 생성
    if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE/*그래픽 */,
        nullptr, iFlag, nullptr, 0, D3D11_SDK_VERSION,
        m_Device.GetAddressOf()/*Device포인터의 포인터*/, &eLevel,
        m_Context.GetAddressOf()/*Context포인터의 포인터*/)))
    {
        MessageBox(nullptr, L"Device, Context 생성 실패", L"Device 초기화 에러", MB_OK);
        return E_FAIL;
    }

    //SwapChain 생성
    if (FAILED(CreateSwapChain()))
    {
        MessageBox(nullptr, L"SwapChain 생성 실패", L"Device 초기화 에러", MB_OK);
        return E_FAIL;
    }

    //View 생성
    if (FAILED(CreateView()))
    {
        MessageBox(nullptr, L"View 생성 실패", L"Device 초기화 에러", MB_OK);
        return E_FAIL;
    }

    //ViewPort 설정 - 보여질 윈도우
    m_ViewPort.TopLeftX = 0.f;
    m_ViewPort.TopLeftY = 0.f;

    Vec2 vWindowResol = CEngine::GetInst()->GetWindowResolution();
    m_ViewPort.Width = vWindowResol.x;
    m_ViewPort.Height = vWindowResol.y;

    m_ViewPort.MinDepth = 0;
    m_ViewPort.MaxDepth = 1;

    m_Context->RSSetViewports(1, &m_ViewPort);

    //RasterizerState  생성
    if (FAILED(CreateRasterizerState()))
    {
        MessageBox(nullptr, L"레스터라이져 스테이트 생성 실패", L"Device 초기화 에러", MB_OK);
        return E_FAIL;
    }

    // DepthStencilState 생성
    if (FAILED(CreateDepthStencilState()))
    {
        MessageBox(nullptr, L"DepthStencilState 생성 실패", L"Device 초기화 에러", MB_OK);
        return E_FAIL;
    }

    // BlendState 생성
    if (FAILED(CreateBlendState()))
    {
        MessageBox(nullptr, L"BlendState 생성 실패", L"Device 초기화 에러", MB_OK);
        return E_FAIL;
    }

    //샘플러 생성
    if (FAILED(CreateSampler()))
    {
        MessageBox(nullptr, L"샘플러 생성 실패", L"Device 초기화 에러", MB_OK);
        return E_FAIL;
    }

    // 상수버퍼 생성
    CreateConstBuffer();

    return S_OK; //E_FAIL;
}


int CDevice::CreateSwapChain()
{
    //SwapChain & Buffer 설정
    DXGI_SWAP_CHAIN_DESC tDesc = {};

    tDesc.OutputWindow = m_hWnd;                            // 출력 윈도우
    tDesc.Windowed = true;                                  // 창모드, 전체화면 모드

    tDesc.BufferCount = 1;                                  // 버퍼의 개수2개여야 되는데 1로 쓰게 된다 DX12부터는 2로 됨
    tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    // 버퍼의 용도
    tDesc.BufferDesc.Width = (UINT)m_vRenderResolution.x;                // 버퍼 가로길이
    tDesc.BufferDesc.Height = (UINT)m_vRenderResolution.y;              // 버퍼 세로길이
    tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;   // RGB 색상을 0~1로 변형(shader에서 사용)
    tDesc.BufferDesc.RefreshRate.Denominator = 1;           // 화면 갱신비율(주사율) 분자
    tDesc.BufferDesc.RefreshRate.Numerator = 60;            // 화면 갱신비율(주사율) 분모
    tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;   //미사용
    tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //미사용

    tDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;  //미사용

    tDesc.SampleDesc.Count = 1;                             // 0이 되면 안됨
    tDesc.SampleDesc.Quality = 0;
    tDesc.Flags = 0;

    //SwapChain 생성
    ComPtr<IDXGIDevice> pDXGIDevice;//Comptr은 초기값이 이미 정의 되어있어 초기화 안해도 됨
    ComPtr<IDXGIAdapter> pAdapter;
    ComPtr<IDXGIFactory> pFactory;

    HRESULT hr = S_OK; //HRESULT는 오류 조건 및 경고 조건을 나타내는데 사용되는 데이터 유형
                       //오류인지 아닌지 확인하는거

    hr = m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf());
    hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());
    hr = pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

    hr = pFactory->CreateSwapChain(m_Device.Get(), &tDesc, m_SwapChain.GetAddressOf()); // Factory에 있는 CreateSwapChain을 사용 하려고

    return hr;
}

int CDevice::CreateView()
{
    //스왑 체인의 백 버퍼 중 tex2D에 접근.
    ComPtr<ID3D11Texture2D> tex2D;
    m_SwapChain->GetBuffer(0, _uuidof(ID3D11Texture2D), (void**)tex2D.GetAddressOf());

    // RenderTarget 용 텍스쳐 등록
    m_RTTex = CResMgr::GetInst()->CreateTexture(L"RenderTargetTex", tex2D);

    // DepthStencil 용도 텍스쳐 생성
    m_DSTex = CResMgr::GetInst()->CreateTexture(L"DepthStencilTex"
        , (UINT)m_vRenderResolution.x, (UINT)m_vRenderResolution.y
        , DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL, D3D11_USAGE_DEFAULT);

    return S_OK;
}
    
int CDevice::CreateRasterizerState()
{
    m_RSState[(UINT)RS_TYPE::CULL_BACK] = nullptr;

    D3D11_RASTERIZER_DESC Desc = {};
    
    //Cull Front
    Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
    Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf());

    //Cull None
    Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
    Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());

    //Wire Frame
    Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
    Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
    DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());

    return S_OK;
}

int CDevice::CreateDepthStencilState()
{
    // Less
    m_DSState[(UINT)DS_TYPE::LESS] = nullptr;

    // Less Equal
    D3D11_DEPTH_STENCIL_DESC Desc = {};
    Desc.DepthEnable = true;
    Desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    Desc.StencilEnable = false;
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::LESS_EQUAL].GetAddressOf());

    // Greater
    Desc.DepthEnable = true;
    Desc.DepthFunc = D3D11_COMPARISON_GREATER;
    Desc.StencilEnable = false;
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::GREATER].GetAddressOf());

    // Greater Equal
    Desc.DepthEnable = true;
    Desc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
    Desc.StencilEnable = false;
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::GREATER_EQUAL].GetAddressOf());

    // No Write
    Desc.DepthEnable = true;
    Desc.DepthFunc = D3D11_COMPARISON_LESS;
    Desc.StencilEnable = false;
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_WRITE].GetAddressOf());

    // NoTest NoWrite
    Desc.DepthEnable = false;
    Desc.StencilEnable = false;
    Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf());


    return S_OK;
}

int CDevice::CreateBlendState()
{
    // No Blend
    m_BSState[(UINT)BS_TYPE::DEFAULT] = nullptr;

    // Mask
    D3D11_BLEND_DESC Desc = {};
    Desc.AlphaToCoverageEnable = true;
    Desc.IndependentBlendEnable = false;

    Desc.RenderTarget[0].BlendEnable = true;
    Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;

    Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

    Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    DEVICE->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::MASK].GetAddressOf());


    // Alpha Blend   
    Desc.AlphaToCoverageEnable = false;
    Desc.IndependentBlendEnable = false;

    Desc.RenderTarget[0].BlendEnable = true;
    Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;//D3D11_BLEND_SRC_ALPHA
    Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

    Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;//D3D11_BLEND_ONE
    Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

    Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;//D3D11_COLOR_WRITE_ENABLE_ALL
    DEVICE->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::ALPHA_BLEND].GetAddressOf());

    // One One
    Desc.AlphaToCoverageEnable = false;
    Desc.IndependentBlendEnable = false;

    Desc.RenderTarget[0].BlendEnable = true;
    Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

    Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

    Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    DEVICE->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::ONE_ONE].GetAddressOf());

    return S_OK;
}


int CDevice::CreateSampler()
{
    D3D11_SAMPLER_DESC tSamDesc = {};

    tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.Filter = D3D11_FILTER_ANISOTROPIC;//축소, 확대 및 밉 레벨 샘플링에 이방성 보간법을 사용합니다.
    DEVICE->CreateSamplerState(&tSamDesc, m_Sampler[0].GetAddressOf());

    tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    tSamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;//축소, 확대 및 밉 레벨 샘플링에 포인트 샘플링을 사용합니다.
    DEVICE->CreateSamplerState(&tSamDesc, m_Sampler[1].GetAddressOf());

    CONTEXT->VSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());//vs 에 샘플러를 등록한다
    CONTEXT->HSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->DSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->GSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
    CONTEXT->PSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());

    CONTEXT->VSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->HSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->DSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->GSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
    CONTEXT->PSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());

    return S_OK;
}

void CDevice::ClearTarget(float(&_color)[4])
{
    m_Context->ClearRenderTargetView(m_RTTex->GetRTV().Get(), _color);
    m_Context->ClearDepthStencilView(m_DSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

}

void CDevice::CreateConstBuffer()//버퍼는 gpu 저장공간
{
    m_arrConstBuffer[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer((UINT)CB_TYPE::TRANSFORM);
    m_arrConstBuffer[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(tTransform), 1);

    m_arrConstBuffer[(UINT)CB_TYPE::MATERIAL] = new CConstBuffer((UINT)CB_TYPE::MATERIAL);
    m_arrConstBuffer[(UINT)CB_TYPE::MATERIAL]->Create(sizeof(tMtrlConst), 1);

    m_arrConstBuffer[(UINT)CB_TYPE::GLOBAL] = new CConstBuffer((UINT)CB_TYPE::GLOBAL);
    m_arrConstBuffer[(UINT)CB_TYPE::GLOBAL]->Create(sizeof(tGlobal), 1);
}