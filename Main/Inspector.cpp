#include "pch.h"
#include "Inspector.h"

#include "Engine/GameObject.h"

#include "TransformGUI.h"
#include "Animator2DGUI.h"

Inspector::Inspector() : GUI("Inspector", "##Inspector")
	, m_TargetObject(nullptr)
	, m_TargetResource(nullptr)
	, m_ComGUI{}
{
	m_ComGUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformGUI;
	AddChild(m_ComGUI[(UINT)COMPONENT_TYPE::TRANSFORM]);

	m_ComGUI[(UINT)COMPONENT_TYPE::ANIMATOR2D] = new Animator2DGUI;
	AddChild(m_ComGUI[(UINT)COMPONENT_TYPE::ANIMATOR2D]);
}

Inspector::~Inspector()
{
}

void Inspector::RenderUpdate()
{
	if (nullptr == m_TargetObject)
		return;

	const std::wstring& tName = m_TargetObject->GetName();

	ImGui::Text(std::string(tName.begin(), tName.end()).c_str());
}

void Inspector::SetTargetObject(GameObject* _object)
{
	m_TargetObject = _object;

	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; i++)
	{
		if (nullptr != m_ComGUI[i])
			m_ComGUI[i]->SetTargetObject(_object);
	}
}

void Inspector::SetTargetResource(Resouce* _resouce)
{
}
