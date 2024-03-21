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
	m_ProjType(PROJ_TYPE::ORTHOGRAPHIC), // �ʱ� �������
	m_iLayerMask(0),
	m_iCamIdx(-1)
{
	Vec2 vRenderResol = CDevice::GetInst()->GetRenderResolution();
	m_fAspectRatio = vRenderResol.x / vRenderResol.y; // ��Ⱦ�� ����� ȭ�� �ػ��� ������ �����ش�.
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
	// View ��� ���
	// ==============
	m_matView = XMMatrixIdentity(); //�ʱ�ȭ

	//ī�޶� ��ǥ�� �������� �̵�
	Vec3 vCamPos = Transform()->GetRelativePos();//ī�޶� ��ǥ = ������Ʈ ��� ��ġ
	Matrix matViewTrans = XMMatrixTranslation(-vCamPos.x, -vCamPos.y, -vCamPos.z);//ī�޶� �����̴� �ݴ�������� �ʱ�ȭ

	// ī�޶� �ٶ󺸴� ������ Z ��� �����ϰ� ����� ȸ�� ����� ����
	Matrix matViewRot = XMMatrixIdentity(); //���� ����� ��ȯ

	//���� 90�� ��� = ���� ���->������� ���� ���Ѵ�. ������ �Ʒ��� ���� �缱�� �������� ��ġ��Ų��
	//���� = ���� ���� ����� ���� ����x����xcos��Ÿ
	//ȸ������� ���⺤���� ������̴�
	Vec3 vR = Transform()->GetWorldDir(DIR_TYPE::RIGHT);// camera ������ ���� dir�� �޾ƿ´�(����)->���� dir
	Vec3 vU = Transform()->GetWorldDir(DIR_TYPE::UP);
	Vec3 vF = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	matViewRot._11 = vR.x;	matViewRot._12 = vU.x;	matViewRot._13 = vF.x;
	matViewRot._21 = vR.y;	matViewRot._22 = vU.y;	matViewRot._23 = vF.y;
	matViewRot._31 = vR.z;	matViewRot._32 = vU.z;	matViewRot._33 = vF.z;

	m_matView = matViewTrans * matViewRot; //�� �࿭�� view�̵� * view ȸ��
}

void CCamera::CalcProjMat()
{
	//���� ��� ��� �ϱ� ���� ī�޶� z���� �ٶ󺸰� �־�� �Ǳ⿡ �̵�*ȸ�� ����� �� ���̴�.
	//�̵����� ȸ���� �����̰�
	//�̵��Ŀ� ȸ���� �����̴�
	//x��ȸ�� = pitch
	//y��ȸ�� = yaw
	//z��ȸ�� = roll
	//ȸ������� 4x4���� 3x3�� �����ϰ��ִ�

	// =============
	// ���� ��� ���
	// =============
	m_matProj = XMMatrixIdentity();
	//--------------
	//���� ���� = ��󵵸� ����ȭ�ؼ� NDC�� �����Ű�� �� //���ٰ��� ���� �������
	//����: �ȼ�1�� scale1�̶� ����

	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
	{
		// ���� ����
		Vec2 vResolution = CDevice::GetInst()->GetRenderResolution();
		m_matProj = XMMatrixOrthographicLH(vResolution.x * (1.f / m_fScale), vResolution.y * (1.f / m_fScale), 1.f, 10000.f);//�������� ����� �ִ� �Լ�. �Ÿ��� 1���� 10000����
		//����� Ŭ���� ��鿡 �ִ� frustum�� �ʺ�, ����� Ŭ���� ��鿡 �ִ� frustum�� ����, ����� Ŭ���� �������� �Ÿ�, ���Ÿ� Ŭ���� �������� �Ÿ�
	}
	else
	{
		// ���� ����
		m_matProj = XMMatrixPerspectiveFovLH(XM_PI / 2.f, m_fAspectRatio, 1.f, 10000.f);//�޼� ��ǥ��� ���� ���� ����� �ۼ�
		//���� ������ ����� �þ߰�, �� ���� X:Y�� ��Ⱦ��(16:9), �Ͼ� Ŭ���� �������� �Ÿ�, ���Ÿ� Ŭ���� �������� �Ÿ�
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
	// ���� ������ �з����� ����
	clear();

	// ���� ���� �����ͼ� �з�
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// ���̾� ����ũ Ȯ��
		if (m_iLayerMask & (1 << i))
		{
			CLayer* pLayer = pCurLevel->GetLayer(i);
			const vector<CGameObject*>& vecObject = pLayer->GetObjects();

			for (size_t j = 0; j < vecObject.size(); ++j)
			{
				CRenderComponent* pRenderCom = vecObject[j]->GetRenderComponent();

				// ������ ����� ���� ������Ʈ�� ����
				if (	nullptr == pRenderCom
					||	nullptr == pRenderCom->GetMaterial()
					||	nullptr == pRenderCom->GetMaterial()->GetShader())
					continue;

				// ���̴� �����ο� ���� �з�
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
	//��� ������Ʈ
	g_transform.matView = m_matView;
	g_transform.matProj = m_matProj;

	// ���̴� �����ο� ���� ���������� �׸���
	render_opaque();
	render_mask();
	render_transparent();

	//PostProcess = �� ó��
	//���ó���ϰ� ������ �ϼ��� ����� ���̴� ���ҽ��� ���ε��ؼ��ϴ� �߰� �۾�.
	//1.����Ÿ���̶� ������ �ػ��� �ؽ��ĸ� ī���� �� �ִ� �ؽ�ó�� ����Ŵ�
	//����Ÿ���� ������ �ͼ� ���纻�� ���
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