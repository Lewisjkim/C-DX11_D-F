#include "pch.h"
#include "CLoginScript.h"
#include <Engine/CKeyMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CEventMgr.h>
#include <Engine\components.h>
#include <Engine/CEngine.h>


CLoginScript::CLoginScript() : 
	CScript((UINT)SCRIPT_TYPE::LOGINSCRIPT)
{
}

CLoginScript::~CLoginScript()
{
}

void CLoginScript::begin()
{
	MeshRender()->GetDynamicMaterial();
}

void CLoginScript::tick()
{
	Vec3 BTpos = Transform()->GetRelativePos();
	Vec3 BTscale = Transform()->GetRelativeScale();

	HWND m_hwnd = CEngine::GetInst()->GetMainWnd();
	Vec2 resolution = CEngine::GetInst()->GetWindowResolution();
	resolution = resolution / 2;
	POINT mousePos;

	GetCursorPos(&mousePos);
	ScreenToClient(m_hwnd, &mousePos);
	mousePos.x -= resolution.x;
	mousePos.y -= resolution.y;
	if (mousePos.y < 0)
		mousePos.y = abs(mousePos.y);
	else
		mousePos.y = -abs(mousePos.y);
	//���콺�� ���� ��ġ�� ���� ��ư�� LT~RB�� ��ġ�� �ְ�
	float L = BTpos.x - BTscale.x / 2;
	float R = BTpos.x + BTscale.x / 2;
	float T =  BTpos.y - BTscale.y / 2;
	float B = BTpos.y + BTscale.y / 2;
	if (mousePos.x > BTpos.x - BTscale.x / 2 && mousePos.x < BTpos.x + BTscale.x/2
		&& mousePos.y > BTpos.y - BTscale.y / 2 && mousePos.y < BTpos.y + BTscale.y/2)
	{
		if (KEY_TAP(KEY::LBTN))//���콺 Ŭ���� �ϸ�
		{

		}
		/*{*/
			/*CLevel* pNewLevel = CLevelSaveLoad::Stop(L"Level\\PracticeRoom.lv", LEVEL_STATE::PLAY);
			pNewLevel->SetName(L"PracticeRoom");
			CLevelMgr::GetInst()->ChangeLevel(pNewLevel);
		}*/
	}
	
} 

void CLoginScript::BeginOverlap(CCollider2D* _Other)
{
	
}

