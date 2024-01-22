#include "pch.h"
#include "Collider2DGUI.h"

#include <Engine/GameObject.h>
#include <Engine/Collider2D.h>

Collider2DGUI::Collider2DGUI() : ComponentGUI("Collider2D", "##Collider2DGUI", COMPONENT_TYPE::COLLIDER2D)
{
	SetSize(ImVec2(0, 140));
}

Collider2DGUI::~Collider2DGUI()
{
}

void Collider2DGUI::RenderUpdate()
{
	if (nullptr == GetTargetObject())
		return;

	ComponentGUI::RenderUpdate();

	Collider2D* targetCollider = GetTargetObject()->GetCollider2D();
	//DebugDraw
	bool debugDraw = targetCollider->GetDebugDraw();
	//offset
	Vec2 pos = targetCollider->GetOffsetPos();
	Vec2 scale = targetCollider->GetOffsetScale();
	//absolute
	bool absolute = targetCollider->IsAbsolute();
	//type
	COLLIDER2D_TYPE type = targetCollider->GetCollider2DType();

	ImGui::Text("Debug Draw"); ImGui::SameLine(); ImGui::Checkbox("##Collider2DGUIDebugDraw", &debugDraw);

	float speed = 0.1f;
	if (absolute)
		speed = 1.f;

	ImGui::Text("Offset");
	ImGui::Text("Position"); ImGui::SameLine(); ImGui::DragFloat2("##Collider2DGUIOffsetPos", pos, speed);
	ImGui::Text("Scale   "); ImGui::SameLine(); ImGui::DragFloat2("##Collider2DGUIOffsetScale", scale, speed);
	ImGui::Spacing();

	ImGui::Text("Absolute"); ImGui::SameLine(); ImGui::Checkbox("##Collider2DGUIAbsolute", &absolute);
	ImGui::Spacing();

	targetCollider->SetDeubgDraw(debugDraw);
	targetCollider->SetOffsetPos(pos);
	targetCollider->SetOffsetScale(scale);
	targetCollider->SetAbsolute(absolute);

	//ImGui::Text("Type");
	//
	//std::vector<std::string> typeVector;
	//typevec
	//
	//if (ImGui::BeginCombo("combo 1", nullptr))
	//{
	//	for (int n = 0; n < IM_ARRAYSIZE(items); n++)
	//	{
	//		const bool is_selected = (item_current_idx == n);
	//		if (ImGui::Selectable(items[n], is_selected))
	//			item_current_idx = n;
	//
	//		// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
	//		if (is_selected)
	//			ImGui::SetItemDefaultFocus();
	//	}
	//	ImGui::EndCombo();
	//}
}