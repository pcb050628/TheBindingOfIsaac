#include "pch.h"
#include "ComponentGUI.h"

#include <Engine/GameObject.h>

ComponentGUI::ComponentGUI(const std::string& _strName, const std::string& _strID, COMPONENT_TYPE _type) : GUI(_strName, _strID)
	, m_TargetObject(nullptr)
	, m_Type(_type)
{
}

ComponentGUI::~ComponentGUI()
{

}

void ComponentGUI::RenderUpdate()
{
	ImGui::SeparatorText(GetName().c_str());
}

void ComponentGUI::SetTargetObject(GameObject* _object)
{
	m_TargetObject = _object;

	if (nullptr == m_TargetObject)
	{
		Deactivate();
	}
	else
	{
		if (nullptr == m_TargetObject->GetComponent(m_Type))
		{
			Deactivate();
		}
		else
		{
			Activate();
		}
	}
}
