#include "pch.h"
#include "CEngine.h"

#include "CDevice.h"
#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CCollisionMgr.h"
#include "CRenderMgr.h"
#include "CEventMgr.h"
#include "CFontMgr.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "/DirectX_Practice/DirectX_/Project/Client/CLevelSaveLoad.h"
CEngine::CEngine()	:
	m_hWnd(nullptr)
{
}

CEngine::~CEngine()
{
	//delete nameobj;
}

int CEngine::init(HWND _hWnd, UINT _iWidth, UINT _iHeight)
{
	// 메인 윈도우 핸들
	m_hWnd = _hWnd;
	m_vResolution = Vec2((float)_iWidth, (float)_iHeight);

	// 해상도에 맞는 작업영역 크기 조정
	RECT rt = { 0, 0, (int)_iWidth, (int)_iHeight };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(m_hWnd, true);

	// Device 초기화 - 
	// 1. 윈도우 해상도와 RT해상도 동일화, 
	// 2. Device,Context 생성, SwapChain, View생성(RTview, DTTexture, DepthStencil), 
	// 3. 출력할 타겟 설정, 뷰 포트(렌더 될 곳)설정, 스테이트 생성(RState, DState, BState) 
	// 4. 샘플러 생성(anisotrophic&point), 상수 버퍼 생성(Transform, Material CBuffer)
	if (FAILED(CDevice::GetInst()->init(m_hWnd, _iWidth, _iHeight)))
	{
		MessageBox(nullptr, L"Device 초기화 실패", L"에러", MB_OK);
		return E_FAIL;
	}
	
	// Manager 초기화
	CPathMgr::GetInst()->init();		// CurrentDirectory를 bin//content로 설정
	CKeyMgr::GetInst()->init();			// 사용할 키들을 vector에 등록한다
	CTimeMgr::GetInst()->init();		// 1초당 카운팅 증가량 시간 측정, 일정 기간동안 몇번 진동 했는지

	CResMgr::GetInst()->init();			// Mesh 생성 & Res 추가, Shader ", Material ", 프리펩 ", 텍스쳐 불러오기
	CRenderMgr::GetInst()->init();		// Light2DBuffer structure(구조화) 버퍼 생성
	CFontMgr::GetInst()->init();
	CLevelMgr::GetInst()->init();		// 레이어 설정, 오브젝트 생성해서 현재 레벨에 추가, 충돌 시킬 레이어 설정

	return S_OK;
}

void CEngine::progress()
{
	tick();
	render();

	//event 처리
	CEventMgr::GetInst()->tick();		// 이벤트가 들어오면 다음 프레임에 실행한다
										// 오브젝트 생성, 삭제, 자식추가, 리소스 삭제, 레벨 변경
}

void CEngine::tick()
{
	CResMgr::GetInst()->tick();
	CTimeMgr::GetInst()->tick();		// dt사용하기 위해
	CKeyMgr::GetInst()->tick();			// 실시간 입력을 받기 위해

	CSound::g_pFMOD->update();

	CLevelMgr::GetInst()->tick();		// 1.오브젝트 clear, 2.script tick, 
										// 3.transform,collider2d,camera, light, animator finaltick
	CCollisionMgr::GetInst()->tick();	// 레이어간 충돌검사->오브젝트간 충돌검사->충돌체간 충돌검사
}

void CEngine::render()
{
	CRenderMgr::GetInst()->render(); //카메라가 비추고 있는 오브젝트를 쉐이더 도메인에 따라 분류한다
																// opaque, mask, transparent, postprocess, ui
																//행렬 업데이트, 쉐이더 도메인에 따라서 순차적으로 그리기
	CTimeMgr::GetInst()->render();//fps표시
	
	static wchar_t szBuff2[256] = {};
	static wchar_t addwchar[256] = {};
	//
	//if (L"CharacterSelect" == CLevelMgr::GetInst()->GetCurLevel()->GetName())
	//{
	//	if (KEY_TAP(KEY::A))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"a");
	//		else
	//		{
	//			swprintf_s(addwchar, L"a");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::B))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"b");
	//		else
	//		{
	//			swprintf_s(addwchar, L"b");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::C))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"c");
	//		else
	//		{
	//			swprintf_s(addwchar, L"c");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::D))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"d");
	//		else
	//		{
	//			swprintf_s(addwchar, L"d");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::E))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"e");
	//		else
	//		{
	//			swprintf_s(addwchar, L"e");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::F))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"f");
	//		else
	//		{
	//			swprintf_s(addwchar, L"f");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::G))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"g");
	//		else
	//		{
	//			swprintf_s(addwchar, L"g");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::H))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"h");
	//		else
	//		{
	//			swprintf_s(addwchar, L"h");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::I))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"i");
	//		else
	//		{
	//			swprintf_s(addwchar, L"i");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::J))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"j");
	//		else
	//		{
	//			swprintf_s(addwchar, L"j");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::K))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"k");
	//		else
	//		{
	//			swprintf_s(addwchar, L"k");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::L))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"l");
	//		else
	//		{
	//			swprintf_s(addwchar, L"l");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::M))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"m");
	//		else
	//		{
	//			swprintf_s(addwchar, L"m");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::N))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"n");
	//		else
	//		{
	//			swprintf_s(addwchar, L"n");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::O))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"o");
	//		else
	//		{
	//			swprintf_s(addwchar, L"o");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::P))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"p");
	//		else
	//		{
	//			swprintf_s(addwchar, L"p");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::Q))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"q");
	//		else
	//		{
	//			swprintf_s(addwchar, L"q");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::R))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"r");
	//		else
	//		{
	//			swprintf_s(addwchar, L"r");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::S))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"s");
	//		else
	//		{
	//			swprintf_s(addwchar, L"s");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::T))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"t");
	//		else
	//		{
	//			swprintf_s(addwchar, L"t");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::U))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"u");
	//		else
	//		{
	//			swprintf_s(addwchar, L"u");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::V))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"v");
	//		else
	//		{
	//			swprintf_s(addwchar, L"v");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::W))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"w");
	//		else
	//		{
	//			swprintf_s(addwchar, L"w");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::X))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"x");
	//		else
	//		{
	//			swprintf_s(addwchar, L"x");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::Y))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"y");
	//		else
	//		{
	//			swprintf_s(addwchar, L"y");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::Z))
	//	{
	//		if (szBuff2 == nullptr)
	//			swprintf_s(szBuff2, L"z");
	//		else
	//		{
	//			swprintf_s(addwchar, L"z");
	//			wcscat_s(szBuff2, addwchar);
	//		}
	//	}
	//	if (KEY_TAP(KEY::BACKSPACE))
	//	{
	//		if (szBuff2 != nullptr)
	//		{
	//			wstring wstr = szBuff2;
	//			wstr.pop_back();
	//			swprintf_s(szBuff2, wstr.c_str());
	//		}
	//	}
	//	CFontMgr::GetInst()->DrawFont(szBuff2, 570, 600, 25, FONT_RGBA(255, 255, 255, 255));
	//	gameid= szBuff2;
	//	
	//	if (KEY_TAP(KEY::LBTN))
	//	{
	//		CLevel* pNewLevel = CLevelSaveLoad::Stop(L"Level\\SeriaRoom.lv",LEVEL_STATE::PLAY);
	//		pNewLevel->SetName(L"SeriaRoom");
	//		//CLevelMgr::GetInst()->ChangeLevel(pNewLevel);
	//		CLevelMgr::GetInst()->ChangeLevel(pNewLevel);
	//		Ptr<CSound> pSound = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Map\\characterselectbgm.ogg");
	//		pSound->Stop();
	//		Ptr<CSound> pSound2 = CResMgr::GetInst()->FindRes<CSound>(L"Sound\\Map\\seria_gate.ogg");
	//		pSound2->Play(0, 0.5f, false);
	//	}
	//}
	//
}									