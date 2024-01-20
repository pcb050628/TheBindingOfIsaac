#include "pch.h"
#include "EditorGUI.h"

#include "AnimEditorGUI.h"

EditorGUI::EditorGUI() : GUI("Editor", "##EditorGUI")
{
	m_AnimEditor = new AnimEditorGUI;
}

EditorGUI::~EditorGUI()
{
}

void EditorGUI::RenderUpdate()
{
	bool anim = m_AnimEditor->IsActive();
	ImGui::Checkbox("##EditorGUIAnimEditorCheckBox", &anim);
	anim ? m_AnimEditor->Activate() : m_AnimEditor->Deactivate();

	if(ImGui::BeginTabBar("Editor##EditorGUITabBar"))
	{
		m_AnimEditor->RenderUpdate();

		ImGui::EndTabBar();
	}
}
