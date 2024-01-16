#include "pch.h"
#include "Inspector.h"

#include "Engine/GameObject.h"

Inspector::Inspector() : GUI("Inspector", "##Inspector")
	, m_TargetObject(nullptr)
	, m_TargetResource(nullptr)
	, m_TransformGUI(nullptr)
	, m_MeshRendererGUI(nullptr)
	, m_CameraGUI(nullptr)
	, m_Animator2DGUI(nullptr)
	, m_Collider2DGUI(nullptr)
	, m_Light2DGUI(nullptr)
{
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
	//m_TransformGUI;
}

void Inspector::SetTargetResource(Resouce* _resouce)
{
}
