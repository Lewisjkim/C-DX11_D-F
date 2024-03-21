#include "pch.h"
#include "CCamera.h"

#include "CDevice.h"
#include "CRenderMgr.h"
#include "CTransform.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CRenderComponent.h"
#include "CMaterial.h"
#include "CGraphicsShader.h"


CCamera::CCamera()	:
	CComponent(COMPONENT_TYPE::CAMERA),
	m_fAspectRatio(1.f),
	m_fScale(1.f),
	m_ProjType(PROJ_TYPE::ORTHOGRAPHIC), // 초기 투영방식
	m_iLayerMask(0),
	m_iCamIdx(-1)
{
	Vec2 vRenderResol = CDevice::GetInst()->GetRenderResolution();
	m_fAspectRatio = vRenderResol.x / vRenderResol.y; // 종횡비를 출력할 화면 해상도의 비율로 맞춰준다.
	SetName(L"Camera");
}

CCamera::CCamera(const CCamera& _Other)	:
	CComponent(_Other),
	m_fAspectRatio(_Other.m_fAspectRatio),
	m_fScale(_Other.m_fScale),
	m_ProjType(_Other.m_ProjType),
	m_iLayerMask(_Other.m_iLayerMask),
	m_iCamIdx(-1)

{
}

CCamera::~CCamera()
{
}

void CCamera::begin()
{
	if (-1 != m_iCamIdx)
	{
		CRenderMgr::GetInst()->RegisterCamera(this, m_iCamIdx);//0=main, else = sub
	}
}

void CCamera::finaltick()
{
	CalcViewMat();

	CalcProjMat();
}


void CCamera::CalcViewMat()
{
	// ==============
	// View 행렬 계산
	// ==============
	m_matView = XMMatrixIdentity(); //초기화

	//카메라 좌표를 원점으로 이동
	Vec3 vCamPos = Transform()->GetRelativePos();//카메라 좌표 = 오브젝트 출력 위치
	Matrix matViewTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);//카메라가 움직이는 반대방향으로 초기화

	// 카메라가 바라보는 방향을 Z 축과 평행하게 만드는 회전 행렬을 적용
	Matrix matViewRot = XMMatrixIdentity(); //단위 행렬을 반환

	//서로 90인 행렬 = 직교 행렬->역행렬을 쉽게 구한다. 오른쪽 아래로 가는 사선을 기준으로 전치시킨다
	//내적 = 같은 성분 행렬의 곱셈 길이x길이xcos세타
	//회전행렬은 방향벡터의 역행렬이다
	Vec3 vR = Transform()->GetWorldDir(DIR_TYPE::RIGHT);// camera 본인의 현재 dir을 받아온다(기존)->월드 dir
	Vec3 vU = Transform()->GetWorldDir(DIR_TYPE::UP);
	Vec3 vF = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	matViewRot._11 = vR.x;	matViewRot._12 = vU.x;	matViewRot._13 = vF.x;
	matViewRot._21 = vR.y;	matViewRot._22 = vU.y;	matViewRot._23 = vF.y;
	matViewRot._31 = vR.z;	matViewRot._32 = vU.z;	matViewRot._33 = vF.z;

	m_matView = matViewTrans * matViewRot; //뷰 행열은 view이동 * view 회전
}

void CCamera::CalcProjMat()
{
	//투영 행렬 계산 하기 전에 카메라가 z축을 바라보고 있어야 되기에 이동*회전 행렬을 한 것이다.
	//이동전에 회전은 자전이고
	//이동후에 회전은 공전이다
	//x축회전 = pitch
	//y축회전 = yaw
	//z축회전 = roll
	//회전행렬은 4x4에서 3x3을 차지하고있다

	// =============
	// 투영 행렬 계산
	// =============
	m_matProj = XMMatrixIdentity();
	//--------------
	//직교 투영 = 행상도를 정규화해서 NDC로 압축시키는 것 //원근감이 없는 투영방식
	//장점: 픽셀1이 scale1이랑 같다

	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
	{
		// 직교 투영
		Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
		m_matProj = XMMatrixOrthographicLH(vResolution.x * (1.f / m_fScale), vResolution.y * (1.f / m_fScale), 1.f, 10000.f);//직교투영 만들어 주는 함수. 거리는 1부터 10000까지
		//가까운 클리핑 평면에 있는 frustum의 너비, 가까운 클리핑 평면에 있는 frustum의 높이, 가까운 클리핑 평면까지의 거리, 원거리 클리핑 평면까지의 거리
	}
	else
	{
		// 원근 투영
		m_matProj = XMMatrixPerspectiveFovLH(XM_PI / 2.f, m_fAspectRatio, 1.f, 10000.f);//왼손 좌표계로 원근 투영 행렬을 작성
		//라디안 단위의 하향식 시야각, 뷰 공간 X:Y의 종횡비(16:9), 니어 클리핑 평면까지의 거리, 원거리 클리핑 평면까지의 거리
	}
}


void CCamera::SetLayerMask(int _iLayer, bool _Visible)
{
	if (_Visible)
	{
		m_iLayerMask |= 1 << _iLayer;
	}
	else
	{
		m_iLayerMask &= ~(1 << _iLayer);
	}
}

void CCamera::SetLayerMaskAll(bool _Visible)
{
	if (_Visible)
		m_iLayerMask = 0xffffffff;
	else
		m_iLayerMask = 0;
}

void CCamera::SetCameraIndex(int _idx)
{
	m_iCamIdx = _idx;
	CRenderMgr::GetInst()->RegisterCamera(this, m_iCamIdx);
}

void CCamera::SortObject()
{
	// 이전 프레임 분류정보 제거
	clear();

	// 현재 레벨 가져와서 분류
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// 레이어 마스크 확인
		if (m_iLayerMask & (1 << i))
		{
			CLayer* pLayer = pCurLevel->GetLayer(i);
			const vector<CGameObject*>& vecObject = pLayer->GetObjects();

			for (size_t j = 0; j < vecObject.size(); ++j)
			{
				CRenderComponent* pRenderCom = vecObject[j]->GetRenderComponent();

				// 렌더링 기능이 없는 오브젝트는 제외
				if (	nullptr == pRenderCom
					||	nullptr == pRenderCom->GetMaterial()
					||	nullptr == pRenderCom->GetMaterial()->GetShader())
					continue;

				// 쉐이더 도메인에 따른 분류
				SHADER_DOMAIN eDomain = pRenderCom->GetMaterial()->GetShader()->GetDomain();
				switch (eDomain)
				{
				case SHADER_DOMAIN::DOMAIN_OPAQUE:
					m_vecOpaque.push_back(vecObject[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_MASK:
					m_vecMask.push_back(vecObject[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
					m_vecTransparent.push_back(vecObject[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_POSTPROCESS:
					m_vecPost.push_back(vecObject[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_UI:
					m_vecUI.push_back(vecObject[j]);
					break;
				}
			}
		}
	}
}

void CCamera::render()
{
	//행렬 업데이트
	g_transform.matView = m_matView;
	g_transform.matProj = m_matProj;

	// 쉐이더 도메인에 따라서 순차적으로 그리기
	render_opaque();
	render_mask();
	render_transparent();

	//PostProcess = 후 처리
	//흑백처리하고 싶으면 완성된 장면을 셰이더 리소스로 바인딩해서하는 추가 작업.
	//1.랜더타깃이랑 동일한 해상도의 텍스쳐를 카피할 수 있는 텍스처를 만들거다
	//랜더타깃을 가지고 와서 복사본을 뜬다
	render_postprocess();
	
	// UI
	render_ui();
}

void CCamera::clear()
{
	m_vecOpaque.clear();
	m_vecMask.clear();
	m_vecTransparent.clear();
	m_vecPost.clear();
	m_vecUI.clear();
}

void CCamera::render_opaque()
{
	for (size_t i = 0; i < m_vecOpaque.size(); ++i)
	{
		m_vecOpaque[i]->render();
	}
}

void CCamera::render_mask()
{
	for (size_t i = 0; i < m_vecMask.size(); ++i)
	{
		m_vecMask[i]->render();
	}
}

void CCamera::render_transparent()
{
	for (size_t i = 0; i < m_vecTransparent.size(); ++i)
	{
		m_vecTransparent[i]->render();
	}
}

void CCamera::render_postprocess()
{
	for (size_t i = 0; i < m_vecPost.size(); ++i)
	{
		CRenderMgr::GetInst()->CopyRenderTarget();
		m_vecPost[i]->render();
	}
}

void CCamera::render_ui()
{
	for (size_t i = 0; i < m_vecUI.size(); ++i)
	{
		m_vecUI[i]->render();
	}
}

void CCamera::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_fAspectRatio, sizeof(float), 1, _File);
	fwrite(&m_fScale   , sizeof(float), 1, _File);
	fwrite(&m_ProjType, sizeof(UINT), 1, _File);
	fwrite(&m_iLayerMask, sizeof(UINT), 1, _File);
	fwrite(&m_iCamIdx, sizeof(int), 1, _File);
}

void CCamera::LoadFromLevelFile(FILE* _File)
{
	fread(&m_fAspectRatio, sizeof(float), 1, _File);
	fread(&m_fScale, sizeof(float), 1, _File);
	fread(&m_ProjType, sizeof(UINT), 1, _File);
	fread(&m_iLayerMask, sizeof(UINT), 1, _File);
	fread(&m_iCamIdx, sizeof(int), 1, _File);
}