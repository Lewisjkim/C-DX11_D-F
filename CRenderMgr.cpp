#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

#include "CCamera.h"
#include "CLight2D.h"
#include "CResMgr.h"

CRenderMgr::CRenderMgr() : 
	m_Light2DBuffer(nullptr),
	RENDER_FUNC(nullptr),
	m_pEditorCam(nullptr)
{
	Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
	//copy용 텍스쳐 생성
	m_RTCopyTex = CResMgr::GetInst()->CreateTexture(L"RTCopyTex", (UINT)vResolution.x, (UINT)vResolution.y, 
		DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT);
	CResMgr::GetInst()->FindRes<CMaterial>(L"GrayMtrl")->SetTexParam(TEX_0, m_RTCopyTex);
	CResMgr::GetInst()->FindRes<CMaterial>(L"DistortionMtrl")->SetTexParam(TEX_0, m_RTCopyTex);
}

CRenderMgr::~CRenderMgr()
{
	if (nullptr != m_Light2DBuffer)
		delete m_Light2DBuffer;
}

void CRenderMgr::init()
{
	// Light2DBuffer 구조화 버퍼 생성
	m_Light2DBuffer = new CStructuredBuffer;
	m_Light2DBuffer->Create(sizeof(tLightInfo), 10, SB_TYPE::READ_ONLY, true);
}

void CRenderMgr::render()
{
	// 렌더링 시작 == 회색
	float arrColor[4] = { 0.2f, 0.2f, 0.2f, 1.f };
	CDevice::GetInst()->ClearTarget(arrColor);

	// 출력 타겟 지정  
	CDevice::GetInst()->OMSet();

	// 광원 및 전역 데이터 업데이트 및 바인딩
	UpdateData();

	
	//렌더 함수 호출
	(this->*RENDER_FUNC)();

	// 광원 해제
	Clear();
}

void CRenderMgr::render_play()
{
	// 카메라 기준 렌더링
	for (size_t i = 0; i < m_vecCam.size(); i++)
	{
		if (nullptr == m_vecCam[i])
			continue;

		m_vecCam[i]->SortObject();	//카메라가 비추고 있는 오브젝트를 쉐이더 도메인에 따라 분류한다
									// opaque, mask, transparent, postprocess, ui
		m_vecCam[i]->render();		//행렬 업데이트, 쉐이더 도메인에 따라서 순차적으로 그리기
	}
}

void CRenderMgr::render_editor()
{
	m_pEditorCam->SortObject();
	m_pEditorCam->render();
}

int CRenderMgr::RegisterCamera(CCamera* _Cam, int _idx)
{
	if (m_vecCam.size() <= _idx)
	{
		m_vecCam.resize(_idx + 1);
	}

	m_vecCam[_idx] = _Cam;
	return _idx;
}

void CRenderMgr::SetRenderFunc(bool _IsPlay)
{
	if (_IsPlay)
		RENDER_FUNC = &CRenderMgr::render_play;
	else
		RENDER_FUNC = &CRenderMgr::render_editor;
}

void CRenderMgr::CopyRenderTarget()
{
	Ptr<CTexture> pRTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");
	CONTEXT->CopyResource(m_RTCopyTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());//해상도가 같을 경우에만 가능
}

void CRenderMgr::UpdateData()
{
	// GlobalData 에 광원 개수정보 세팅
	GlobalData.Light2DCount = m_vecLight2D.size();

	// 구조화버퍼의 크기가 모자라면 더 크게 새로 만든다.
	if (m_Light2DBuffer->GetElementCount() < m_vecLight2D.size())
	{
		m_Light2DBuffer->Create(sizeof(tLightInfo), m_vecLight2D.size(), SB_TYPE::READ_ONLY, true);
	}

	// 구조화버퍼로 광원 데이터를 옮긴다.
	m_Light2DBuffer->SetData(m_vecLight2D.data(), sizeof(tLightInfo) * m_vecLight2D.size());
	m_Light2DBuffer->UpdateData(12, PIPELINE_STAGE::PS_PIXEL);


	// 전역 상수 데이터 바인딩
	CConstBuffer* pGlobalBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
	pGlobalBuffer->SetData(&GlobalData, sizeof(tGlobal));
	pGlobalBuffer->UpdateData();
	pGlobalBuffer->UpdateData_CS();//compute 쪽에도 구조체 정보를 넘겨줘야 되서 추가
}

void CRenderMgr::Clear()//clear light2dvec
{
	m_vecLight2D.clear();

}
