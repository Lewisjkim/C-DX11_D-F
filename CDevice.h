#pragma once
// Device �뵵: DX11 Lib����
#include "ptr.h"
#include "CTexture.h"

class CConstBuffer;

class CDevice	:
	public CSingleton<CDevice>
{
	SINGLE(CDevice)
private:
	HWND							m_hWnd;			//Buffer���� �׷����ϱ� ������

	ComPtr<ID3D11Device>			m_Device;		// GPU �޸� �Ҵ�
	ComPtr<ID3D11DeviceContext>		m_Context;		// GPU ����, ������, ���� ����

	ComPtr<IDXGISwapChain>			m_SwapChain;	// FBuffer�� BBuffer�� ��ü���ִ� swap chain

	//���� Ŭ���� �������� �����Ͽ� ���� �ؽ��ĸ� ���� �� ������ �並 ������ �� �ִ�
	Ptr<CTexture>					m_RTTex;		// �׷��� ��Ҹ� ǥ���ϴ� �ؽ���
	Ptr<CTexture>					m_DSTex;		// ���̸� ǥ���ϴ� texture
	
	// Sampler
	ComPtr<ID3D11SamplerState>		m_Sampler[2];  //sampling�� �Ϸ��� �ʿ�
	//RsterizerState
	ComPtr<ID3D11RasterizerState>	m_RSState[(UINT)RS_TYPE::END];
	//DepthStencilState
	ComPtr<ID3D11DepthStencilState>	m_DSState[(UINT)DS_TYPE::END];
	//BlendState
	ComPtr<ID3D11BlendState>		m_BSState[(UINT)BS_TYPE::END];

	D3D11_VIEWPORT					m_ViewPort;		//�׷����� ���� ����(����ü)

	// ���� Ÿ�� �ػ�(Buffer)
	Vec2							m_vRenderResolution;
	CConstBuffer*					m_arrConstBuffer[(UINT)CB_TYPE::END];

public:
	int init(HWND _hwnd, UINT _iWidth, UINT _iHeight);
	void ClearTarget(float(&_color)[4]);
	void OMSet() { m_Context->OMSetRenderTargets(1, m_RTTex->GetRTV().GetAddressOf(), m_DSTex->GetDSV().Get()); }
	void Present() { m_SwapChain->Present(0, 0); } //present�� �������� �̹����� ����ڿ��� �����ϴ� �Լ�.

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
	ID3D11Device* GetDevice() { return m_Device.Get(); } // Device �����͸� ��ȯ
	ID3D11DeviceContext* GetDeviceContext() { return m_Context.Get(); } // Context �����͸� ��ȯ
	CConstBuffer* GetConstBuffer(CB_TYPE _Type) { return m_arrConstBuffer[(UINT)_Type]; }

	ComPtr<ID3D11RasterizerState> GetRSState(RS_TYPE _Type) { return m_RSState[(UINT)_Type]; }
	ComPtr<ID3D11DepthStencilState> GetDSState(DS_TYPE _Type) { return m_DSState[(UINT)_Type]; }
	ComPtr<ID3D11BlendState> GetBSState(BS_TYPE _Type) { return m_BSState[(UINT)_Type]; }

};

