#include "pch.h"
#include "CTexture.h"
#include "CPathMgr.h"
#include "CGameObject.h"
#include "CAnimator2D.h"
#include "CAnim2D.h"
#include "CDevice.h"

CTexture::CTexture(bool _bEngine)
	: CRes(RES_TYPE::TEXTURE, _bEngine)
	, m_Desc{}
{
}

CTexture::~CTexture()
{
}

void CTexture::UpdateData(int _iRegisterNum, int _PipelineStage) //오브젝트 텍스쳐 설정한걸 넘길때 사용
{
	m_iRecentNum = _iRegisterNum;

	if (PIPELINE_STAGE::PS_VERTEX & _PipelineStage)
	{
		CONTEXT->VSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
	}

	if (PIPELINE_STAGE::PS_HULL & _PipelineStage)
	{
		CONTEXT->HSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
	}

	if (PIPELINE_STAGE::PS_DOMAIN & _PipelineStage)
	{
		CONTEXT->DSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
	}

	if (PIPELINE_STAGE::PS_GEOMETRY & _PipelineStage)
	{
		CONTEXT->GSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
	}

	if (PIPELINE_STAGE::PS_PIXEL & _PipelineStage)
	{
		CONTEXT->PSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
	}
}

void CTexture::UpdateData_CS(int _iRegisterNum, bool _bShaderRes) //color shader, noise tex 사용
{
	m_iRecentNum = _iRegisterNum;

	if (_bShaderRes)
	{
		CONTEXT->CSSetShaderResources(m_iRecentNum, 1, m_SRV.GetAddressOf());
	}
	else
	{
		UINT i = -1;
		CONTEXT->CSSetUnorderedAccessViews(m_iRecentNum, 1, m_UAV.GetAddressOf(), &i);
	}
}

void CTexture::Clear()
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->VSSetShaderResources(m_iRecentNum, 1, &pSRV);
	CONTEXT->HSSetShaderResources(m_iRecentNum, 1, &pSRV);
	CONTEXT->DSSetShaderResources(m_iRecentNum, 1, &pSRV);
	CONTEXT->GSSetShaderResources(m_iRecentNum, 1, &pSRV);
	CONTEXT->PSSetShaderResources(m_iRecentNum, 1, &pSRV);
}

void CTexture::Clear(int _iRegisterNum)
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->VSSetShaderResources(_iRegisterNum, 1, &pSRV);
	CONTEXT->HSSetShaderResources(_iRegisterNum, 1, &pSRV);
	CONTEXT->DSSetShaderResources(_iRegisterNum, 1, &pSRV);
	CONTEXT->GSSetShaderResources(_iRegisterNum, 1, &pSRV);
	CONTEXT->PSSetShaderResources(_iRegisterNum, 1, &pSRV);
}

void CTexture::Clear_CS(bool _bShaderRes)
{
	if (_bShaderRes)
	{
		ID3D11ShaderResourceView* pSRV = nullptr;
		CONTEXT->CSSetShaderResources(m_iRecentNum, 1, &pSRV);
	}
	else
	{
		ID3D11UnorderedAccessView* pUAV = nullptr;
		UINT i = -1;
		CONTEXT->CSSetUnorderedAccessViews(m_iRecentNum, 1, &pUAV, &i);
	}
}

void CTexture::UpdateData()//안쓰는것
{
}


int CTexture::Load(const wstring& _strFilePath)
{
	wchar_t szExt[50] = L"";
	_wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 50);
	wstring strExt = szExt;

	HRESULT hr = S_OK;
	if (L".dds" == strExt || L".DDS" == strExt)//불러오는 파일이 dds,DDS 포맷일경우
	{
		// dds, DDS
		hr = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, m_Image);
	}

	else if (L".tga" == strExt || L".TGA" == strExt)//불러오는 파일이 tga, TGA 포맷일경우
	{
		// tga, TGA
		hr = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
	}

	else//불러오는 파일이 png, jpg, jpeg, bmp 포맷일경우
	{
		// png, jpg, jpeg, bmp
		hr = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, m_Image);
	}

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"리소스 로딩 실패", L"텍스쳐 로딩 실패", MB_OK);
		return E_FAIL;
	}
	//디바이스 스크레치 이미지info, count, metadata, SRV
	//불러온 이미지를 스크레치가 가지고 있다. = m_Image
	hr = CreateShaderResourceView(DEVICE
		, m_Image.GetImages()
		, m_Image.GetImageCount()
		, m_Image.GetMetadata() //info 동일한 크기로 설정해야된다.
		, m_SRV.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"ShaderResourceView 생성 실패", L"텍스쳐 로딩 실패", MB_OK);
		return E_FAIL;
	}

	m_SRV->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());//Texture를 알아보고싶으면
	m_Tex2D->GetDesc(&m_Desc);//Desc정보를 받아보고 싶으면 - CreateShaderResourceView로 만들면 Desc을 우리가 만들지 않기 때문

	return S_OK;
}

//텍스쳐를 원하는 용도로 만든다
int CTexture::Create(UINT _Width, UINT _Height, DXGI_FORMAT _pixelformat
	, UINT _BindFlag, D3D11_USAGE _Usage)
{
	// ID3D11Texture2D 생성
	m_Desc.Format = _pixelformat;

	// 반드시 렌더타겟과 같은 해상도로 설정해야 함
	m_Desc.Width = _Width;
	m_Desc.Height = _Height;
	m_Desc.ArraySize = 1;

	m_Desc.BindFlags = _BindFlag;
	m_Desc.Usage = _Usage;

	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == _Usage)
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	else if (D3D11_USAGE::D3D11_USAGE_STAGING == _Usage)
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	m_Desc.MipLevels = 1;    // 원본만 생성
	m_Desc.SampleDesc.Count = 1;
	m_Desc.SampleDesc.Quality = 0;


	if (FAILED(DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf())))
	{
		return E_FAIL;
	}

	// 바인드 플래그에 맞는 View 를 생성해준다.
	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		if (FAILED(DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf())))
		{
			return E_FAIL;
		}
	}
	else
	{
		if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
		{
			if (FAILED(DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}

		if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			if (FAILED(DEVICE->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}

		if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			if (FAILED(DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), nullptr, m_UAV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}
	}


	return S_OK;
}

int CTexture::Create(ComPtr<ID3D11Texture2D> _tex2D)
{
	m_Tex2D = _tex2D;

	m_Tex2D->GetDesc(&m_Desc);

	// 바인드 플래그에 맞는 View 를 생성해준다.
	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		if (FAILED(DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf())))
		{
			return E_FAIL;
		}
	}
	else
	{
		if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
		{
			if (FAILED(DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}

		if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			if (FAILED(DEVICE->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}

		if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			if (FAILED(DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), nullptr, m_UAV.GetAddressOf())))
			{
				return E_FAIL;
			}
		}
	}

	return S_OK;
}

int CTexture::Save(const wstring& _strRelativePath)
{
	wstring strFilepath = CPathMgr::GetInst()->GetContentPath();
	strFilepath += _strRelativePath;

	// 파일 입출력
	FILE* pFile = nullptr;
	errno_t iErrNum = _wfopen_s(&pFile, strFilepath.c_str(), L"wb");

	if (nullptr == pFile)
	{
		wchar_t szStr[256] = {};
		wsprintf(szStr, L"Animation Save 실패, Error Number : %d", iErrNum);
		MessageBox(nullptr, szStr, L"파일 저장 실패", MB_OK);
		return FALSE;
	}

	// Animation 이름 저장
	fwprintf_s(pFile, L"\n");

	fwprintf_s(pFile, L"[ANIMATION_NAME]\n");
	fwprintf_s(pFile, GetName().c_str());
	fwprintf_s(pFile, L"\n\n");

	// 아틀라스 텍스쳐 키값 저장	
	fwprintf_s(pFile, L"[ATLAS_KEY]\n");
	fwprintf_s(pFile, this->GetKey().c_str());
	fwprintf_s(pFile, L"\n\n");

	fwprintf_s(pFile, L"[ATLAS_PATH]\n");
	fwprintf_s(pFile, this->GetRelativePath().c_str());
	fwprintf_s(pFile, L"\n\n");


	// 프레임 정보 저장
	wchar_t szNum[50] = {};

	fwprintf_s(pFile, L"[FRAME_COUNT]\n");

	CGameObject* obj = new CGameObject;
	const tAnim2DFrm& Frm = obj->Animator2D()->GetCurAnim()->GetCurFrame();
	
	size_t iFrmCount = obj->Animator2D()->GetCurAnim()->GetFramSize();
	_itow_s((int)iFrmCount, szNum, 50, 10);

	fwprintf_s(pFile, szNum);
	fwprintf_s(pFile, L"\n\n");

	for (size_t i = 0; i < iFrmCount; ++i)
	{
		fwprintf_s(pFile, L"[%zd_FRAME]\n", i);

		//// LEFT_TOP
		//fwprintf_s(pFile, L"[LEFT_TOP]\n");
		//fwprintf_s(pFile, L"%.1f %.1f\n", m_vecFrm[i].vLeftTop.x, m_vecFrm[i].vLeftTop.y);

		//// SIZE
		//fwprintf_s(pFile, L"[SIZE]\n");
		//fwprintf_s(pFile, L"%.1f %.1f\n", m_vecFrm[i].vSize.x, m_vecFrm[i].vSize.y);

		//// OFFSET
		//fwprintf_s(pFile, L"[OFFSET]\n");
		//fwprintf_s(pFile, L"%.1f %.1f\n", m_vecFrm[i].vOffset.x, m_vecFrm[i].vOffset.y);

		// DURATION
		fwprintf_s(pFile, L"[DURATION]\n");
		fwprintf_s(pFile, L"%.2f\n", Frm.fDuration);

		fwprintf_s(pFile, L"\n");
	}

	fclose(pFile);
	delete obj;
	return S_OK;
}