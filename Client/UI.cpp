#include "pch.h"
#include "UI.h"
//#include "InspectorUI.h"
//#include "ComponentUI.h"
#include <Engine/CGameObject.h>


UI::UI(const string& _ID)
	: m_strID(_ID)
	, m_ParentUI(nullptr)
	, m_Modal(false)
	, m_Active(false)
{

}

UI::~UI()
{
	Safe_Del_Vec(m_vecChildUI);

}

void UI::finaltick()
{
	if (!m_Active)//활성화 되지 않았다면
		return;

	string strFullName = m_strName + m_strID;

	// 부모 UI가 없다면
	if (nullptr == m_ParentUI)
	{
		// 모달리스
		if (!m_Modal)
		{
			ImGui::Begin(strFullName.c_str(), &m_Active); //component ui를 활성화 상태로 이름을 지어서 출력해라

			render_update();

			for (size_t i = 0; i < m_vecChildUI.size(); ++i)
			{
				// 자식UI 가 비활성화 상태면 건너뛴다.
				if (!m_vecChildUI[i]->IsActive())
					continue;

				m_vecChildUI[i]->finaltick();

				// 자식 UI 간의 구분선
				if (i != m_vecChildUI.size() - 1)
					ImGui::Separator();
			}

			ImGui::End();
		}

		// 모달
		else
		{
			ImGui::SetNextWindowPos(m_vPopupPos);
			ImGui::SetNextWindowSize(m_vSize);

			ImGui::OpenPopup(strFullName.c_str());
			if (ImGui::BeginPopupModal(strFullName.c_str(), &m_Active))
			{
				render_update();

				for (size_t i = 0; i < m_vecChildUI.size(); ++i)
				{
					// 자식UI 가 비활성화 상태면 건너뛴다.
					if (!m_vecChildUI[i]->IsActive())
						continue;

					m_vecChildUI[i]->finaltick();

					// 자식 UI 간의 구분선
					if (i != m_vecChildUI.size() - 1)
						ImGui::Separator();
				}

				ImGui::EndPopup();
			}
		}
	}

	// 부모ui가 있다면(자식 UI)
	else
	{	
		ImGui::BeginChild(strFullName.c_str(), m_vSize);

		render_update();

		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->finaltick();

			if (i != m_vecChildUI.size() - 1)
				ImGui::Separator();
		}

		ImGui::EndChild();
	}
}