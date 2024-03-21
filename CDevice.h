#pragma once
// Device 용도: DX11 Lib관리
#include "ptr.h"
#include "CTexture.h"

class CConstBuffer;

class CDevice	:
	public CSingleton<CDevice>
{
	SINGLE(CDevice)
private:
	HWND							m_hWnd;			//Buffer에서 그려야하기 때문에

	ComPtr<ID3D11Device>			m_Device;		// GPU 메모리 할당
	ComPtr<ID3D11DeviceContext>		m_Context;		// GPU 제어, 렌더링, 동작 수행

	ComPtr<IDXGISwapChain>			m_SwapChain;	// FBuffer와 BBuffer를 교체해주는 swap chain

	//만든 클래스 형식으로 변경하여 직접 텍스쳐를 만들 때 복수의 뷰를 설정할 수 있다
	Ptr<CTexture>					m_RTTex;		// 그려질 장소를 표현하는 텍스쳐
	Ptr<CTexture>					m_DSTex;		// 깊이를 표현하는 texture
	
	// Sampler
	ComPtr<ID3D11SamplerState>		m_Sampler[2];  //sampling을 하려면 필요
	//RsterizerState
	ComPtr<ID3D11RasterizerState>	m_RSState[(UINT)RS_TYPE::END];
	//DepthStencilState
	ComPtr<ID3D11DepthStencilState>	m_DSState[(UINT)DS_TYPE::END];
	//BlendState
	ComPtr<ID3D11BlendState>		m_BSState[(UINT)BS_TYPE::END];

	D3D11_VIEWPORT					m_ViewPort;		//그려지는 곳의 정보(구조체)

	// 렌더 타겟 해상도(Buffer)
	Vec2							m_vRenderResolution;
	CConstBuffer*					m_arrConstBuffer[(UINT)CB_TYPE::END];

public:
	int init(HWND _hwnd, UINT _iWidth, UINT _iHeight);
	void ClearTarget(float(&_color)[4]);
	void OMSet() { m_Context->OMSetRenderTargets(1, m_RTTex->GetRTV().GetAddressOf(), m_DSTex->GetDSV().Get()); }
	void Present() { m_SwapChain->Present(0, 0); } //present는 렌더링된 이미지를 사용자에게 제공하는 함수.

	Vec2 GetRenderResolution() { return m_vRenderResolution; }

private:
	int CreateSwapChain();
	int CreateView();
	int CreateRasterizerState();
	int CreateBlendState();
	int CreateDepthStencilState();
	int CreateSampler();
	void CreateConstBuffer();

public:
	ID3D11Device* GetDevice() { return m_Device.Get(); } // Device 포인터를 반환
	ID3D11DeviceContext* GetDeviceContext() { return m_Context.Get(); } // Context 포인터를 반환
	CConstBuffer* GetConstBuffer(CB_TYPE _Type) { return m_arrConstBuffer[(UINT)_Type]; }

	ComPtr<ID3D11RasterizerState> GetRSState(RS_TYPE _Type) { return m_RSState[(UINT)_Type]; }
	ComPtr<ID3D11DepthStencilState> GetDSState(DS_TYPE _Type) { return m_DSState[(UINT)_Type]; }
	ComPtr<ID3D11BlendState> GetBSState(BS_TYPE _Type) { return m_BSState[(UINT)_Type]; }

};

