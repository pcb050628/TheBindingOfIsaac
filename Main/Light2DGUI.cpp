#include "pch.h"
#include "Light2DGUI.h"

#include <Engine/GameObject.h>
#include <Engine/Light2D.h>

Light2DGUI::Light2DGUI() : ComponentGUI("Light2D", "##Light2DGUI", COMPONENT_TYPE::LIGHT2D)
{
	SetSize(ImVec2(0, 200));

	m_lightType.push_back("DIRECTIONAL");
	m_lightType.push_back("POINT");
	m_lightType.push_back("SPOT");
}

Light2DGUI::~Light2DGUI()
{
}

void Light2DGUI::RenderUpdate()
{
	if (nullptr == GetTargetObject())
		return;

	ComponentGUI::RenderUpdate();

	Light2D* targetLight = GetTargetObject()->GetLight2D();
	tLightInfo& info = targetLight->m_Info;

	//int	LightType;
	LIGHT_TYPE type = (LIGHT_TYPE)info.LightType;
	if (ImGui::BeginCombo("##Light2DGUILightTypeCombo", nullptr, ImGuiComboFlags_NoPreview))
	{
		for (int i = 0; i < (UINT)LIGHT_TYPE::END; i++)
		{
			char buf[50];
			_itoa_s(i, buf, 10);
			std::string label = m_lightType[i];
			label += "##Light2DGUILightType";
			label += buf;
			bool is_selected = (((int)type) == i);
			if (ImGui::Selectable(label.c_str(), is_selected))
				info.LightType = i;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	Vec4 color		= info.vColor;
	Vec4 specular	= info.vSpecular;
	Vec4 ambient	= info.vAmbient;

	color *= 255;
	specular *= 255;
	ambient *= 255;

	ImGui::Text("Color   ");	 ImGui::SameLine(); ImGui::DragFloat4("##Light2DGUIColorDragFloat4", color, 0.1f, 0.f, 255);
	ImGui::Text("Specular"); ImGui::SameLine(); ImGui::DragFloat4("##Light2DGUISpecularDragFloat4", specular, 0.1f, 0.f, 255);
	ImGui::Text("Ambient ");  ImGui::SameLine(); ImGui::DragFloat4("##Light2DGUIAmbientDragFloat4", ambient, 0.1f, 0.f, 255);

	color /= 255;
	specular /= 255;
	ambient /= 255;

	Vec4 zero = Vec4(0.f);
	Vec4 one = Vec4(1.f);

	if (color.x < 0)
		color.x = 0;
	if (color.y < 0)
		color.y = 0;
	if (color.z < 0)
		color.z = 0;
	if (color.w < 0)
		color.w = 0;
	if (color.x > 1)
		color.x = 1;
	if (color.y > 1)
		color.y = 1;
	if (color.z > 1)
		color.z = 1;
	if (color.w > 1)
		color.w = 1;

	if (specular.x < 0)
		specular.x = 0;
	if (specular.y < 0)
		specular.y = 0;
	if (specular.z < 0)
		specular.z = 0;
	if (specular.w < 0)
		specular.w = 0;
	if (specular.x > 1)
		specular.x = 1;
	if (specular.y > 1)
		specular.y = 1;
	if (specular.z > 1)
		specular.z = 1;
	if (specular.w > 1)
		specular.w = 1;

	if (ambient.x < 0)
		ambient.x = 0;
	if (ambient.y < 0)
		ambient.y = 0;
	if (ambient.z < 0)
		ambient.z = 0;
	if (ambient.w < 0)
		ambient.w = 0;
	if (ambient.x > 1)
		ambient.x = 1;
	if (ambient.y > 1)
		ambient.y = 1;
	if (ambient.z > 1)
		ambient.z = 1;
	if (ambient.w > 1)
		ambient.w = 1;

	info.vColor = color;
	info.vSpecular = specular;
	info.vAmbient = ambient;

	float radius = info.fRadius;
	float angle = info.fAngle;
	ImGui::Text("Radius"); ImGui::SameLine(); ImGui::DragFloat("##Light2DGUIRadiusDragFloat", &radius);
	ImGui::Text("Angle "); ImGui::SameLine(); ImGui::DragFloat("##Light2DGUIAngleDragFloat", &angle);
	info.fRadius = radius;
	info.fAngle = angle;
}