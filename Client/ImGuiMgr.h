#pragma once
#include <Engine/CSingleton.h>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"


class UI;
// ========
// Delegate
// ========

typedef void (UI::* UI_DELEGATE)(void);
typedef void (UI::* UI_DELEGATE_1)(DWORD_PTR);
typedef void (UI::* UI_DELEGATE_2)(DWORD_PTR, DWORD_PTR);

class ImGuiMgr :
    public CSingleton<ImGuiMgr>
{
    SINGLE(ImGuiMgr);
private:
    HWND                m_hMainHwnd;
    map<string, UI*>    m_mapUI; //inspector, listui 등등
    
    HANDLE                      m_hObserver;

public:
    void init(HWND _hWnd);
    void progress();

public:
    UI* FindUI(const string& _UIName);
    HWND GetMainHwnd() { return m_hMainHwnd; }

private:
    void CreateUI();
    void ObserveContent();

    void begin();
    void tick(); // ui가 각자 할 일들
    void finaltick();
    void render();
};

