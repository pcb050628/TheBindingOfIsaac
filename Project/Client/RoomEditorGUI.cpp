#include "pch.h"
#include "RoomEditorGUI.h"

#include <Engine\Room.h>

RoomEditorGUI::RoomEditorGUI() : GUI("RoomEditor", "##RoomEditorGUI")
	, m_EditRoom(nullptr)
{
}

RoomEditorGUI::~RoomEditorGUI()
{
}

void RoomEditorGUI::RenderUpdate()
{
	bool active = IsActive();
	if (active && ImGui::BeginTabItem("RoomEditor##RoomEditorGUITabBar", &active))
	{


		ImGui::EndTabItem();
	}
}

void RoomEditorGUI::Activate()
{
	if (!m_EditRoom)
		m_EditRoom = new Room;
}
