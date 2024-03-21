#include "pch.h"
#include "CCameraMoveScript.h"

#include <Engine/CDevice.h>
#include <Engine/CTransform.h>
#include <Engine/CCamera.h>
#include <Engine/CGameObject.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include "/DirectX_Practice/DirectX_/Project/Client/CLevelSaveLoad.h"

CCameraMoveScript::CCameraMoveScript()	:
	CScript((UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT),
	m_fCamSpeed(100.f)
{
	
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::tick()
{
	CLevel* curlevel = CLevelMgr::GetInst()->GetCurLevel();
	if(curlevel->GetState()==LEVEL_STATE::STOP)
		curlevel->ChangeState(LEVEL_STATE::PLAY);
	if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
		Camera2DMove();
	else
		Camera3DMove();
}

void CCameraMoveScript::begin()
{
	
}


void CCameraMoveScript::Camera2DMove()
{
	// 키 입력에 따른 카메라 이동
	Vec3 vPos = Transform()->GetRelativePos();
	CLevel* curlevel = CLevelMgr::GetInst()->GetCurLevel();
	
	LEVEL_STATE curState = curlevel->GetState();
	bool isPlay = false;
	if (LEVEL_STATE::PLAY == curState)
	{
		//플레이어를 따라다니게 한다
		CLayer* playerLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(2);
		Vec3 playerPos;
		vector<CGameObject*> vecObj = playerLayer->GetParentObject();
		for (size_t i = 0; i < vecObj.size(); i++)//플레이어를 찾아 플레이어 위치를 구하고
		{
			CGameObject* player = vecObj[i];
			playerPos = player->Transform()->GetRelativePos();
		}
		if (CLevelMgr::GetInst()->GetCurLevel()->GetLayer(1))
		{
			CLayer* mapLayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(1);
			vector<CGameObject*> vecmapObj = mapLayer->GetParentObject();
			Vec2 mapsize;
			for (size_t i = 0; i < vecObj.size(); i++)//맵의 사이즈도 구한다
			{
				CGameObject* map = vecmapObj[i];
				mapsize = map->Transform()->GetRelativeScale();
			}

			Vec2 camsize = CDevice::GetInst()->GetRenderResolution();
			float LTPoint = -(mapsize.x / 2 - camsize.x / 2);
			float RTPoint = (mapsize.x - camsize.x / 2) - mapsize.x / 2;
			if (LTPoint < playerPos.x < RTPoint)//카메라 위치 제한
			{
				if (playerPos.x < LTPoint)
					vPos.x = LTPoint;
				else if (playerPos.x > RTPoint)
					vPos.x = RTPoint;
				else
					vPos.x = playerPos.x;
			}
		}
		
		
		
		if (KEY_TAP(KEY::Q))
		{
			CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();
			CurLevel->ChangeState(LEVEL_STATE::STOP);
		}

		Transform()->SetRelativePos(vPos);
	}
	
	else
	{
		if (KEY_TAP(KEY::SPACE ))
		{
			CLevelSaveLoad::Play(L"Level\\Temp.lv", curlevel);
			curlevel->ChangeState(LEVEL_STATE::PLAY);
		}

		float fSpeed = m_fCamSpeed;
		if (KEY_PRESSED(KEY::LSHIFT))
			fSpeed *= 5.f;

		if (KEY_PRESSED(KEY::_2))
		{
			vPos.y += DT * fSpeed;
		}

		if (KEY_PRESSED(KEY::_3))
		{
			vPos.y -= DT * fSpeed;
		}

		if (KEY_PRESSED(KEY::_1))
		{
			vPos.x -= DT * fSpeed;
		}

		if (KEY_PRESSED(KEY::_4))
		{
			vPos.x += DT * fSpeed;
		}

		if (KEY_PRESSED(KEY::_5))
		{
			float fScale = Camera()->GetScale();
			fScale += DT * 1.f;
			Camera()->SetScale(fScale);
		}

		if (KEY_PRESSED(KEY::_6))
		{
			float fScale = Camera()->GetScale();
			fScale -= DT * 1.f;
			Camera()->SetScale(fScale);
		}
		Transform()->SetRelativePos(vPos);
	}
	
	CGameObject* test = this->GetOwner();
	
}

void CCameraMoveScript::Camera3DMove()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vRot = Transform()->GetRelativeRot();

	Vec3 vFront = Transform()->GetRelativeDir(DIR_TYPE::FRONT);
	Vec3 vUp = Transform()->GetRelativeDir(DIR_TYPE::UP);
	Vec3 vRight = Transform()->GetRelativeDir(DIR_TYPE::RIGHT);

	float fSpeed = m_fCamSpeed;

	if (KEY_PRESSED(KEY::LSHIFT))
		fSpeed *= 5.f;

	if (KEY_PRESSED(KEY::W))
	{
		vPos += DT * vFront * fSpeed;
	}

	if (KEY_PRESSED(KEY::S))
	{
		vPos -= DT * vFront * fSpeed;
	}

	if (KEY_PRESSED(KEY::A))
	{
		vPos -= DT * vRight * fSpeed;
	}

	if (KEY_PRESSED(KEY::D))
	{
		vPos += DT * vRight * fSpeed;
	}


	if (KEY_PRESSED(KEY::RBTN))
	{
		Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();
		vRot.y += DT * vMouseDir.x * 5.f;
		vRot.x -= DT * vMouseDir.y * 5.f;
	}

	Transform()->SetRelativePos(vPos);
	Transform()->SetRelativeRot(vRot);

}