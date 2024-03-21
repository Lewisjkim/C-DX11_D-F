#include "pch.h"
#include "Test.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CPlayerScript.h"

#include "CDevice.h" // device와 deviceContext를 사용하기 위해
#include "CPathMgr.h" // 쉐이더 파일 경로 찾기 위해서
#include "CKeyMgr.h"
#include "CTimeMgr.h"

#include "ptr.h"
#include "CResMgr.h"
#include "CMesh.h"
#include "CGraphicsShader.h"

CGameObject* g_Obj = nullptr;

CGraphicsShader* g_pShader = nullptr;

void Init()
{
	g_Obj = new CGameObject;
	g_Obj->AddComponent(new CTransform);
	g_Obj->AddComponent(new CMeshRender);
	g_Obj->AddComponent(new CPlayerScript);

	Ptr<CMesh> RectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
	g_Obj->MeshRender()->SetMesh(RectMesh.Get());
	g_Obj->MeshRender()->SetShader(g_pShader);
}

void Tick()
{
	g_Obj->tick();
	g_Obj->finaltick();
}

void Render()
{
	g_Obj->render();
}

void Release()
{
	delete g_Obj;
	delete g_pShader;
}
