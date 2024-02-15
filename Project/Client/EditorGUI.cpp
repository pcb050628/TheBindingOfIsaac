#include "pch.h"
#include "EditorGUI.h"

#include "AnimEditorGUI.h"
#include "RoomEditorGUI.h"

EditorGUI::EditorGUI() : GUI("Editor", "##EditorGUI")
{
	m_AnimEditor = new AnimEditorGUI;
	m_RoomEditor = new RoomEditorGUI;
}

EditorGUI::~EditorGUI()
{
	delete m_AnimEditor;
	delete m_RoomEditor;
}

void EditorGUI::RenderUpdate()
{
	bool anim = false;
	ImGui::Text("AnimEditor"); ImGui::SameLine(); ImGui::Checkbox("##EditorGUIAnimEditorCheckBox", &anim);
	anim ? m_AnimEditor->Activate() : m_AnimEditor->Deactivate();

	bool room = true;
	ImGui::Text("RoomEditor"); ImGui::SameLine(); ImGui::Checkbox("##EditorGUIRoomEditorCheckBox", &room);
	room ? m_RoomEditor->Activate() : m_RoomEditor->Deactivate();

	if(ImGui::BeginTabBar("Editor##EditorGUITabBar"))
	{
		m_AnimEditor->RenderUpdate();
		m_RoomEditor->RenderUpdate();

		ImGui::EndTabBar();
	}
}
