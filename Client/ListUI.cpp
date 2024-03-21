#include "pch.h"
#include "ListUI.h"

#include <Engine/CKeyMgr.h>

ListUI::ListUI()
    : UI("##List")
    , m_iSelectedIdx(0)
{
    /*  AddItem("AAA");
      AddItem("BBB");
      AddItem("CCC");  */
}

ListUI::~ListUI()
{
}

void ListUI::finaltick() // list ��ư�� ������ �� ���´�
{
    UI::finaltick(); // active & listui render_update

    // Esc ������ ��Ȱ��ȭ
    if (KEY_TAP(KEY::ESC))
    {
        SetActive(false);

        // ��� UI ��Ŀ�� ����
        ImGui::SetWindowFocus(nullptr);
    }
}

int ListUI::render_update()
{
    // �ֱ� UI �� �۾����� ����� �˾Ƴ���.
    ImVec2 ListUIContentSize = ImGui::GetContentRegionAvail();

    if (ImGui::BeginListBox("##list", ListUIContentSize))
    {
        for (int i = 0; i < m_vecStrData.size(); i++)
        {
            const bool is_selected = (m_iSelectedIdx == i);

            // ����Ʈ ������ ���, ��ȯ���� Ŭ�� True/ False
            if (ImGui::Selectable(m_vecStrData[i].c_str(), is_selected))
            {
                m_iSelectedIdx = i;
            }

            // ListUI�� Ư�� ������(�ε��� i) ������ ����Ŭ���� �߻��ϸ�
            if (ImGui::IsItemHovered() &&
                ImGui::IsMouseDoubleClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))//����Ʈ �����ۿ� �ö� ���� Ŭ����
            {
                SetActive(false);
                ImGui::SetWindowFocus(nullptr);

                // Select Delegate �� ��ϵǾ��ִٸ� ȣ���� �ش�.
                if (m_SelectInst && m_SelectDelegate)
                {
                    //stringData�� ���ڷ� ���� Delegate ȣ��
                    (m_SelectInst->*m_SelectDelegate)((DWORD_PTR)m_vecStrData[i].c_str());
                }
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }


    return TRUE;
}

void ListUI::Reset(const string& _strName, ImVec2 _vSize)
{
    Clear(); // �ѹ� Ŭ���� �� �־ ���� list�� ����
    SetName(_strName); // ���ڷ� ���� strName�� �̸����� ����
    SetSize(_vSize.x, _vSize.y);
    SetActive(true);

    // ��ġ ����
    HWND hWnd = ImGuiMgr::GetInst()->GetMainHwnd();

    RECT rt = {};
    //GetClientRect(hWnd, &rt); // main window�������� ������� ���� ��
    GetWindowRect(hWnd, &rt);

    float CenterX = (float)((rt.left + rt.right) / 2);
    float CenterY = (float)((rt.bottom + rt.top) / 2);

    SetPopupPos(ImVec2(CenterX - _vSize.x / 2.f, CenterY - _vSize.y / 2.f));
}


