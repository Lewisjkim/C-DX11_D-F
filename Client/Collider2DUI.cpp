#include "pch.h"
#include "Collider2DUI.h"
#include <Engine\CGameObject.h>
#include <Engine\CCollider2D.h>

Collider2DUI::Collider2DUI()	:
	ComponentUI("##Colldier2D", COMPONENT_TYPE::COLLIDER2D)
{
	SetName("Collider2D");
}

Collider2DUI::~Collider2DUI()
{
}

int Collider2DUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	ImGui::Text("Object Layer");
	ImGui::SameLine();
	int layerindex = GetTarget()->GetLayerIndex();
	ImGui::InputInt("##ObjLayer", &layerindex, 0, 0);

	ImGui::Text("Absolute");
	ImGui::SameLine();
	bool absolute = GetTarget()->Collider2D()->GetAbsolute();
	ImGui::Checkbox("##Collider2DAbsolute", &absolute);
	GetTarget()->Collider2D()->SetAbsolute(absolute);

	ImGui::Text("OffsetPos  ");
	ImGui::SameLine();
	Vec3 vOffsetPos = GetTarget()->Collider2D()->GetOffsetPos();
	ImGui::DragFloat3("##Col2DOffsetPos", vOffsetPos);
	GetTarget()->Collider2D()->SetOffsetPos(Vec2(vOffsetPos.x, vOffsetPos.y));

	ImGui::Text("OffsetScale");
	ImGui::SameLine();
	Vec3 vOffsetScale = GetTarget()->Collider2D()->GetOffsetScale();
	ImGui::DragFloat3("##Col2DOffsetScale", vOffsetScale);
	GetTarget()->Collider2D()->SetOffsetScale(Vec2(vOffsetScale.x, vOffsetScale.y));

	ImGui::Text("Shape = [ 0: RECT, 1: CIRCLE");
	
	ImGui::InputInt("##Col2DShapeType", &shape);
	if (0 == shape)
	{
		GetTarget()->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::RECT);
	}
	else if (1 == shape)
	{
		GetTarget()->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::CIRCLE);
	}
		
	


	return TRUE;
}
