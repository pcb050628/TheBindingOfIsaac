#include "pch.h"
#include "TransformGUI.h"

#include "Engine/GameObject.h"
#include "Engine/Transform.h"

TransformGUI::TransformGUI() : ComponentGUI("Transform", "##TransformGUI", COMPONENT_TYPE::TRANSFORM)
{
	SetSize(ImVec2(0, 100));
}

TransformGUI::~TransformGUI()
{
}

void TransformGUI::RenderUpdate()
{
	ComponentGUI::RenderUpdate();
	{
		Transform* targetTrasnform = GetTargetObject()->GetTransform();
		Vec3 vPos = targetTrasnform->GetRelativePos();
		Vec3 vScale = targetTrasnform->GetRelativeScale();
		Vec3 vRotation = targetTrasnform->GetRelativeRotation();
		vRotation.ToDegree();

		ImGui::Text("Pos     "); ImGui::SameLine(); ImGui::DragFloat3("##Relative Pos", vPos);
		ImGui::Text("Scale   "); ImGui::SameLine(); ImGui::DragFloat3("##Relative Scale", vScale);
		ImGui::Text("Rotation"); ImGui::SameLine(); ImGui::DragFloat3("##Relative Rotation", vRotation);

		vRotation.ToRadian();
		targetTrasnform->SetRelativePos(vPos);
		targetTrasnform->SetRelativeScale(vScale);
		targetTrasnform->SetRelativeRotation(vRotation);
	}
}