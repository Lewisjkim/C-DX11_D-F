#include "pch.h"
#include "Light2DUI.h"
#include <Engine/CLight2D.h>

Light2DUI::Light2DUI()	:
	ComponentUI("##Light2D", COMPONENT_TYPE::LIGHT2D)
{
	SetName("Light2D");
}

Light2DUI::~Light2DUI()
{
}

int Light2DUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	m_lighttype = GetTarget()->Light2D()->GetLightType();
	//DIRECTIONAL,	// 방향성(광원에 방향값을 넣은것. 고로 위치는 중요하지 않다)
	//	POINT,			// 지점을 중심으로부터 빛이 나가는 빛
	//	SPOT,			// 어느 지점만 빛이 나오는 것
	ImGui::Text("LightType   ");
	ImGui::SameLine();
	ImGui::InputInt("##lighttype", &m_lighttype);
	ImGui::Text("0: Directional 1: Point 2: Spot");
	GetTarget()->Light2D()->SetLightType((LIGHT_TYPE)m_lighttype);
	
	m_lightDir = GetTarget()->Light2D()->GetLightDirection();
	ImGui::Text("LightDir    ");
	ImGui::SameLine();
	ImGui::InputFloat4("##lightdir", f_lightDir);
	m_lightDir = Vec4(f_lightDir[0], f_lightDir[1], f_lightDir[2], f_lightDir[3]);
	GetTarget()->Light2D()->SetLightDirection(m_lightDir);
	
	m_lightDif = GetTarget()->Light2D()->GetLightDiffuse();
	float f_lightDif[4] = { m_lightDif.x, m_lightDif.y, m_lightDif.z, m_lightDif.w };
	ImGui::Text("LightDiffuse");
	ImGui::SameLine();
	ImGui::InputFloat4("##lightdiffuse", f_lightDif);
	m_lightDif = Vec4(f_lightDif[0], f_lightDif[1], f_lightDif[2], f_lightDif[3]);
	GetTarget()->Light2D()->SetLightDiffuse(m_lightDif);
	return false;
	//m_lightAmb = GetTarget()->Light2D()->GetLightAmbient();
	//ImGui::Text("LightAmbient");
	//ImGui::SameLine();
	//ImGui::InputFloat4("##lightambient", f_lightAmb);
	//m_lightAmb = Vec4(f_lightAmb[0], f_lightAmb[1], f_lightAmb[2], f_lightAmb[3]);
	//GetTarget()->Light2D()->SetLightAmbient(m_lightAmb);

	m_lightRadius = GetTarget()->Light2D()->GetRadius();
	ImGui::Text("LightRadius ");
	ImGui::SameLine();
	ImGui::InputFloat("##lightradius", &m_lightRadius);
	GetTarget()->Light2D()->SetRadius(m_lightRadius);

	/*m_lightAngle = GetTarget()->Light2D()->GetAngle();
	ImGui::Text("LightAngle");
	ImGui::SameLine();
	ImGui::InputFloat("##lightangle", &m_lightAngle);
	GetTarget()->Light2D()->SetAngle(m_lightAngle);*/

	return TRUE;
}