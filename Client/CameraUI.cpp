#include "pch.h"
#include "CameraUI.h"
#include <Engine/CCamera.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>


CameraUI::CameraUI()	:
	ComponentUI("##Camera", COMPONENT_TYPE::CAMERA)
{
	SetName("Camera");
}

CameraUI::~CameraUI()
{
}

int CameraUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;
	return TRUE;
	m_index = GetTarget()->Camera()->GetCamIndex();
	ImGui::Text("CamIndex       ");
	ImGui::SameLine();
	ImGui::InputInt("##CamIndex", &m_index);
	GetTarget()->Camera()->SetCameraIndex(m_index);

	//투영방식
	ImGui::Text("Proj Type      ");
	ImGui::SameLine();
	PROJ_TYPE pjType = GetTarget()->Camera()->GetProjType();
	int i_pjType = (int)pjType;
	ImGui::InputInt("##projtype", &i_pjType);
	ImGui::Text("0: ORTHOGRAPHIC 1: PERSPECTIVE");
	GetTarget()->Camera()->SetProjType((PROJ_TYPE)i_pjType);

	if (ImGui::Button("SetAllLayer", ImVec2(100, 18)))
	{
		GetTarget()->Camera()->SetCameraIndex(0);
		GetTarget()->Camera()->SetLayerMaskAll(true);
		GetTarget()->Camera()->SetLayerMask(31, false);
	}
	ImGui::SameLine();
	if (ImGui::Button("SetUILayer", ImVec2(100, 18)))
	{
		GetTarget()->Camera()->SetCameraIndex(1);
		GetTarget()->Camera()->SetLayerMask(31,true);
	}
	return TRUE;

	//메인 카메라 설정
	m_index = GetTarget()->Camera()->GetCamIndex();
	ImGui::Text("CamIndex       ");
	ImGui::SameLine();
	ImGui::InputInt("##CamIndex", &m_index);
	GetTarget()->Camera()->SetCameraIndex(m_index);
	
	//투영방식
	ImGui::Text("Proj Type      "); 
	ImGui::SameLine();
	int type;
	PROJ_TYPE pType = GetTarget()->Camera()->GetProjType();
	 type = (int)pType;
	ImGui::InputInt("##projtype", &type);
	ImGui::Text("0: ORTHOGRAPHIC 1: PERSPECTIVE");
	GetTarget()->Camera()->SetProjType((PROJ_TYPE)type);
	
	//Orthographic에서 사용하는 카메라 배율
	ImGui::Text("Scale for ORTHO");
	ImGui::SameLine();
	float fscale = GetTarget()->Camera()->GetScale();
	ImGui::InputFloat("##scaleforortho", &fscale);
	GetTarget()->Camera()->SetScale(fscale);

	//종횡비
	ImGui::Text("Aspect Ratio   ");
	float aspRatio = GetTarget()->Camera()->GetAspectRatio();
	ImGui::SameLine();
	ImGui::InputFloat("##aspectRatio", &aspRatio);
	GetTarget()->Camera()->SetAspectRatio(aspRatio);

	

	//모든 레이어 체크
	ImGui::Text("Layer Check");
	ImGui::Checkbox("CheckAll", &checkall);
	ImGui::Checkbox("UnCheckAll", &uncheckall);
	for (size_t i = 0; i < MAX_LAYER; i++)
	{
		if (L"" == CLevelMgr::GetInst()->GetCurLevel()->GetLayer(i)->GetName())
			continue;
		CLayer* indlayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(i);
		string name = string(indlayer->GetName().begin(), indlayer->GetName().end());
		//GetTarget()->GetLayerIndex(i)
		ImGui::Checkbox((char*)name.c_str(), &check[i]);
		
	}
	if (checkall)
	{
		/*GetTarget()->Camera()->SetLayerMaskAll(true);*/
		for (size_t i = 0; i < MAX_LAYER; i++)
		{
			check[i] = true;
		}
	}
	if (uncheckall)
	{
		/*GetTarget()->Camera()->SetLayerMaskAll(false);*/
		for (size_t i = 0; i < MAX_LAYER; i++)
		{
			check[i] = false;
		}
	}
	
	if (ImGui::Button("Register", ImVec2(100, 18)))
	{
		for (size_t i = 0; i < MAX_LAYER; i++)
		{
			if (L"" == CLevelMgr::GetInst()->GetCurLevel()->GetLayer(i)->GetName())
				continue;
			CLayer* indlayer = CLevelMgr::GetInst()->GetCurLevel()->GetLayer(i);
			string name = string(indlayer->GetName().begin(), indlayer->GetName().end());

			if(true ==check[i])
				GetTarget()->Camera()->SetLayerMask(i,true);
			else
				GetTarget()->Camera()->SetLayerMask(i, false);

			//check[i] = false;
		}
	}
	return TRUE;
}

