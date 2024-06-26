#include "pch.h"
#include "ImGuiMgr.h"

#include <Engine\CDevice.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CKeyMgr.h>
#include <Engine/CPathMgr.h>

#include <Engine\CGameObject.h>


#include "UI.h"
#include "ParamUI.h"


ImGuiMgr::ImGuiMgr()    :
    m_hMainHwnd(nullptr),
    m_hObserver(nullptr)
{

}

ImGuiMgr::~ImGuiMgr()
{
    // ImGui Release
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    // UI 삭제
    Safe_Del_Map(m_mapUI);
}


void ImGuiMgr::init(HWND _hWnd)
{
    m_hMainHwnd = _hWnd;

    // ImGui 초기화
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(m_hMainHwnd);
    ImGui_ImplDX11_Init(DEVICE, CONTEXT);

    //Tool용 UI 생성
    CreateUI();

    //Content 폴더 변경 점 감시
    //2번쨰 인자는 자식폴더까지 감시할거냐, 3번쨰는 어떻게 감시 
    //감시하면 알림이 오는데 그 알림을 확인할 수 있어야 된다.
    wstring strContentPath = CPathMgr::GetInst()->GetContentPath();
    m_hObserver = FindFirstChangeNotification(strContentPath.c_str(), true, FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_ACTION_REMOVED | FILE_NOTIFY_CHANGE_SIZE | FILE_ACTION_ADDED);
}

void ImGuiMgr::progress()
{
    begin();

    tick();

    finaltick();

    render();

    ObserveContent();
}



void ImGuiMgr::begin()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ParamUI::g_NextId = 0;
}

void ImGuiMgr::tick()
{
    for (const auto& pair : m_mapUI)
    {
        pair.second->tick();
    }
    
}

void ImGuiMgr::finaltick()
{
    // Demo UI
   // ImGui::ShowDemoWindow();

    // InspectorUI
    for (const auto& pair : m_mapUI)
    {
        if (pair.second->IsActive())
        {
            pair.second->finaltick();
        }
    }

    if (KEY_TAP(KEY::ENTER))
        ImGui::SetWindowFocus(nullptr);
}

void ImGuiMgr::render()
{
    // ImGui Rendering
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}


#include "InspectorUI.h"
#include "ContentUI.h"
#include "OutlinerUI.h"
#include "ListUI.h"
#include "MenuUI.h"
#include "AddMaterial.h"

void ImGuiMgr::CreateUI()
{
    UI* pUI = nullptr;

    // InspectorUI
    pUI = new InspectorUI;
    pUI->SetActive(true);
    m_mapUI.insert(make_pair(pUI->GetID(), pUI));

    // ContentUI
    pUI = new ContentUI;
    pUI->SetActive(true);
    m_mapUI.insert(make_pair(pUI->GetID(), pUI));

    // Outliner
    pUI = new OutlinerUI;
    pUI->SetActive(true);
    m_mapUI.insert(make_pair(pUI->GetID(), pUI));

    //MenuUI
    pUI = new MenuUI;
    pUI->SetActive(true);
    m_mapUI.insert(make_pair(pUI->GetID(), pUI));

    // ListUI
    pUI = new ListUI;
    pUI->SetModal(true);
    pUI->SetActive(false);
    m_mapUI.insert(make_pair(pUI->GetID(), pUI));

    //AddMaterial
    pUI = new AddMaterial;
    pUI->SetActive(true);
    m_mapUI.insert(make_pair(pUI->GetID(), pUI));

    for (const auto& pair : m_mapUI)
    {
        pair.second->init();
    }
}

void ImGuiMgr::ObserveContent()
{
    //매 프레임 마다 확인하는데
    DWORD dwWaitStatus = WaitForSingleObject(m_hObserver, 0);
    if (dwWaitStatus == WAIT_OBJECT_0) // 신호를 받은 경우
    {
        //Content 폴더에 변경점이 생겼다
        ContentUI* UI = (ContentUI*)FindUI("##Content");
        UI->Reload();

        FindNextChangeNotification(m_hObserver);//또 감시 해라
    }
}


UI* ImGuiMgr::FindUI(const string& _UIName)
{
    map<string, UI*>::iterator iter = m_mapUI.find(_UIName);

    if (iter == m_mapUI.end())
        return nullptr;

    return iter->second;
}