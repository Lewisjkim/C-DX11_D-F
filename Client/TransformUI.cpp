#include "pch.h"
#include "TransformUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

#include <Engine/CEventMgr.h>

TransformUI::TransformUI()	: 
	ComponentUI("##Transform", COMPONENT_TYPE::TRANSFORM)
{
	SetName("Transform");
}

TransformUI::~TransformUI()
{
}

int TransformUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	wstring wname = GetTarget()->GetName();
	string name = string(wname.begin(), wname.end());
	ImGui::Text("Object Name");
	ImGui::SameLine();
	ImGui::InputText("##objname", (char*)name.c_str(), name.length());
	wstring returnname = wstring(name.begin(), name.end());
	GetTarget()->SetName(returnname);

	// 이벤트 매니저를 통해서 처리한다.
	tEvent evn = {};
	evn.Type = EVENT_TYPE::CHANGE_NAME;
	if(ImGui::IsKeyDown(ImGuiKey_Enter))
		CEventMgr::GetInst()->AddEvent(evn);

	//=================================
	Vec3 vPos = GetTarget()->Transform()->GetRelativePos();
	Vec3 vScale = GetTarget()->Transform()->GetRelativeScale();
	Vec3 vRotation = GetTarget()->Transform()->GetRelativeRot();
	vRotation = (vRotation / XM_PI) * 180.f;
	
	
	ImGui::Text("Absolute");
	absolute = GetTarget()->Transform()->GetAbsolute();
	ImGui::SameLine();
	ImGui::Checkbox("##Absolute", &absolute);
	GetTarget()->Transform()->SetAbsolute(absolute);

	ImGui::Text("Position");
	ImGui::SameLine();
	/*ImGui::DragFloat3("##Relative Position", vPos);*/
	ImGui::DragFloat3("##Relative Position", vPos);

	ImGui::Text("Scale   ");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Scale", vScale);

	ImGui::Text("Rotation");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Rotation", vRotation);

	GetTarget()->Transform()->SetRelativePos(vPos);
	GetTarget()->Transform()->SetRelativeScale(vScale);

	vRotation = (vRotation / 180.f) * XM_PI;
	GetTarget()->Transform()->SetRelativeRot(vRotation);


	return TRUE;
}
