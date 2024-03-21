#include "pch.h"
#include "CEditorObjMgr.h"

#include "CGameObjectEx.h"
#include <Engine\components.h>

#include <Engine\CResMgr.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CTimeMgr.h>
#include <Engine/CKeyMgr.h>

#include <Script/CCameraMoveScript.h>

CEditorObjMgr::CEditorObjMgr()	:
	m_DebugShape{},
	debugshow(false)
{

}

CEditorObjMgr::~CEditorObjMgr()
{
	Safe_Del_Vec(m_vecEditorObj);
	Safe_Del_Array(m_DebugShape);
}

void CEditorObjMgr::init()
{
	// ����� ������ ����
	m_DebugShape[(UINT)SHAPE_TYPE::RECT] = new CGameObjectEx;
	m_DebugShape[(UINT)SHAPE_TYPE::RECT]->AddComponent(new CTransform);
	m_DebugShape[(UINT)SHAPE_TYPE::RECT]->AddComponent(new CMeshRender);
	m_DebugShape[(UINT)SHAPE_TYPE::RECT]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Debug"));
	m_DebugShape[(UINT)SHAPE_TYPE::RECT]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"));

	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE] = new CGameObjectEx;
	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE]->AddComponent(new CTransform);
	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE]->AddComponent(new CMeshRender);
	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE]->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh_Debug"));
	m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE]->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DebugShapeMtrl"));

	// EditorCamObject ����
	CGameObjectEx* pEditorCamObj = new CGameObjectEx;
	pEditorCamObj->AddComponent(new CTransform);
	pEditorCamObj->AddComponent(new CCamera);
	pEditorCamObj->AddComponent(new CCameraMoveScript);

	pEditorCamObj->Camera()->SetLayerMaskAll(true);//��� ���̾� ī�޶� ���̰� ����
	//pEditorCamObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);

	m_vecEditorObj.push_back(pEditorCamObj);
	CRenderMgr::GetInst()->RegisterEditorCamera(pEditorCamObj->Camera());// ������ ī�޶� ���
}



void CEditorObjMgr::progress()
{
	// DebugShape ���� ��������
	vector<tDebugShapeInfo>& vecInfo = CRenderMgr::GetInst()->GetDebugShapeInfo();//�����Ŵ����� �� �����Ӹ��� �߻��ϴ� ������ ��Ƶδ� ��
	m_DebugShapeInfo.insert(m_DebugShapeInfo.end(), vecInfo.begin(), vecInfo.end());//vecInfo�� �ִ� ������ m_DebugShapeInfo�� �Ǳ��
	vecInfo.clear();


	tick();

	render();
}


void CEditorObjMgr::tick()
{
	/*CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	if (KEY_TAP(KEY::P))
	{
		if (pCurLevel->GetState() == LEVEL_STATE::PLAY)
			CLevelMgr::GetInst()->GetCurLevel()->ChangeState(LEVEL_STATE::STOP);
		else
			CLevelMgr::GetInst()->GetCurLevel()->ChangeState(LEVEL_STATE::PLAY);
	}*/




	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->tick();
	}

	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->finaltick();
	}
}

void CEditorObjMgr::render()
{
	for (size_t i = 0; i < m_vecEditorObj.size(); ++i)
	{
		m_vecEditorObj[i]->render();
	}

	// DebugShape Render
	CGameObjectEx* pShapeObj = nullptr;

	vector<tDebugShapeInfo>::iterator iter = m_DebugShapeInfo.begin();
	for (; iter != m_DebugShapeInfo.end();)
	{
		switch (iter->eShape)
		{
		case SHAPE_TYPE::RECT:
			pShapeObj = m_DebugShape[(UINT)SHAPE_TYPE::RECT];
			break;
		case SHAPE_TYPE::CIRCLE:
			pShapeObj = m_DebugShape[(UINT)SHAPE_TYPE::CIRCLE];
			break;
		case SHAPE_TYPE::CUBE:
			break;
		case SHAPE_TYPE::SPHERE:
			break;
		}

		if (iter->matWorld != XMMatrixIdentity())//���� ����� �ƴϿ��ٸ�
		{
			pShapeObj->Transform()->SetWorldMat(iter->matWorld);
		}
		else//���� ����̶��
		{
			pShapeObj->Transform()->SetRelativePos(iter->vWorldPos);
			pShapeObj->Transform()->SetRelativeScale(iter->vWorldScale);
			pShapeObj->Transform()->SetRelativeRot(iter->vWorldRotation);
			pShapeObj->finaltick();//finaltick�� �ؼ� ���� ����� �����
		}
		
		if (KEY_PRESSED(KEY::H))
		{
			pShapeObj->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, &iter->vColor);//�÷� ���� ���޽�Ų��
			pShapeObj->render();
		}
		
		iter->fCurTime += DT;
		if (iter->fMaxTime < iter->fCurTime)
		{
			iter = m_DebugShapeInfo.erase(iter);//�߰��� erase�� �ϱ� ���� iterator�� ���
		}
		else
		{
			++iter;
		}
	}
}
