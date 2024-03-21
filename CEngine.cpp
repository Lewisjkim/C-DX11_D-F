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
	// ���� ������ �ڵ�
	m_hWnd = _hWnd;
	m_vResolution = Vec2((float)_iWidth, (float)_iHeight);

	// �ػ󵵿� �´� �۾����� ũ�� ����
	RECT rt = { 0, 0, (int)_iWidth, (int)_iHeight };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(m_hWnd, true);

	// Device �ʱ�ȭ - 
	// 1. ������ �ػ󵵿� RT�ػ� ����ȭ, 
	// 2. Device,Context ����, SwapChain, View����(RTview, DTTexture, DepthStencil), 
	// 3. ����� Ÿ�� ����, �� ��Ʈ(���� �� ��)����, ������Ʈ ����(RState, DState, BState) 
	// 4. ���÷� ����(anisotrophic&point), ��� ���� ����(Transform, Material CBuffer)
	if (FAILED(CDevice::GetInst()->init(m_hWnd, _iWidth, _iHeight)))
	{
		MessageBox(nullptr, L"Device �ʱ�ȭ ����", L"����", MB_OK);
		return E_FAIL;
	}
	
	// Manager �ʱ�ȭ
	CPathMgr::GetInst()->init();		// CurrentDirectory�� bin//content�� ����
	CKeyMgr::GetInst()->init();			// ����� Ű���� vector�� ����Ѵ�
	CTimeMgr::GetInst()->init();		// 1�ʴ� ī���� ������ �ð� ����, ���� �Ⱓ���� ��� ���� �ߴ���

	CResMgr::GetInst()->init();			// Mesh ���� & Res �߰�, Shader ", Material ", ������ ", �ؽ��� �ҷ�����
	CRenderMgr::GetInst()->init();		// Light2DBuffer structure(����ȭ) ���� ����
	CFontMgr::GetInst()->init();
	CLevelMgr::GetInst()->init();		// ���̾� ����, ������Ʈ �����ؼ� ���� ������ �߰�, �浹 ��ų ���̾� ����

	return S_OK;
}

void CEngine::progress()
{
	tick();
	render();

	//event ó��
	CEventMgr::GetInst()->tick();		// �̺�Ʈ�� ������ ���� �����ӿ� �����Ѵ�
										// ������Ʈ ����, ����, �ڽ��߰�, ���ҽ� ����, ���� ����
}

void CEngine::tick()
{
	CResMgr::GetInst()->tick();
	CTimeMgr::GetInst()->tick();		// dt����ϱ� ����
	CKeyMgr::GetInst()->tick();			// �ǽð� �Է��� �ޱ� ����

	CSound::g_pFMOD->update();

	CLevelMgr::GetInst()->tick();		// 1.������Ʈ clear, 2.script tick, 
										// 3.transform,collider2d,camera, light, animator finaltick
	CCollisionMgr::GetInst()->tick();	// ���̾ �浹�˻�->������Ʈ�� �浹�˻�->�浹ü�� �浹�˻�
}

void CEngine::render()
{
	CRenderMgr::GetInst()->render(); //ī�޶� ���߰� �ִ� ������Ʈ�� ���̴� �����ο� ���� �з��Ѵ�
																// opaque, mask, transparent, postprocess, ui
																//��� ������Ʈ, ���̴� �����ο� ���� ���������� �׸���
	CTimeMgr::GetInst()->render();//fpsǥ��
	
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