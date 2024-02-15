#include "pch.h"
#include "RoomEditorRenderGUI.h"

#include <Engine\ResourceManager.h>
#include <Engine\GameObject.h>
#include <Engine\Room.h>

#include "imgui_internal.h"

#include "ImGuiManager.h"

#include "EditorGUI.h"
#include "RoomEditorGUI.h"

RoomEditorRenderGUI::RoomEditorRenderGUI() : GUI("RoomEditorRender", "##RoomEditorRenderGUI")
	, m_Target(nullptr)
{
	Deactivate();
	SetSize(ImVec2(1050, 630));
}

RoomEditorRenderGUI::~RoomEditorRenderGUI()
{
}

void RoomEditorRenderGUI::RenderUpdate()
{
	if (m_Target && m_Target->IsActive() && nullptr != m_Target->m_EditRoom)
	{
		ImGui::SetWindowSize(ImVec2(1050, 630));
		ImVec2 windowSize = ImGui::GetWindowSize();

		ImGuiIO io = ImGui::GetIO();
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		ImVec2 uiPos = ImGui::GetWindowPos();
		uiPos.y += 20;


		for (int col = 0; col < 15; col++)
		{
			ImVec2 startPos = uiPos;
			startPos += ImVec2((windowSize.x / 15) * col, 0);
			draw_list->AddLine(startPos, startPos + ImVec2(0, windowSize.y), ImColor(1.f, 1.f, 1.f, 1.f));
		}

		for (int row = 0; row < 9; row++)
		{
			ImVec2 startPos = uiPos;
			startPos += ImVec2(0, (windowSize.y / 9) * row);
			draw_list->AddLine(startPos, startPos + ImVec2(windowSize.x, 0), ImColor(1.f, 1.f, 1.f, 1.f));
		}

		ImVec2 testStartPos = uiPos + ImVec2(25, 25);

		for (int col = 0; col < 15; col++)
		{
			for (int row = 0; row < 9; row++)
			{
				if (m_Target->m_EditRoom->m_Info.TileObjectInfo[col][row] == L"")
					continue;

				ImVec2 pos = testStartPos + ImVec2(col * (windowSize.x / 15), row * (windowSize.y / 9));
				std::string obj = ToString(m_Target->m_EditRoom->m_Info.TileObjectInfo[col][row]);
				draw_list->AddText(pos, ImColor(1.f, 0.f, 0.f), obj.c_str());
			}
		}
		
		m_bLeft = ImGui::InvisibleButton("##RoomEditorRenderGUIInvisibleButton", ImGui::GetWindowSize() + ImVec2(0, 20), ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

		if (m_bLeft && m_Target->m_SelectObject != L"")
		{
			ImVec2 mouse_pos(io.MousePos.x, io.MousePos.y);
			mouse_pos -= uiPos;

			UINT col = mouse_pos.x / (windowSize.x / 15);
			UINT row = mouse_pos.y / (windowSize.y / 9);

			Vec2 tile(row, col);

			GameObject* gobj = new GameObject;
			gobj->Load(m_Target->m_SelectObject);
			m_Target->m_EditRoom->AddObjectByTile(gobj, m_Target->m_SelectLayer, tile, true);
			m_Target->m_EditRoom->m_Info.TileObjectInfo[col][row] = m_Target->m_SelectObject;
			m_Target->m_EditRoom->m_Info.TileLayerInfo[col][row] = m_Target->m_SelectLayer;
		}
	}
	else if (m_Target)
	{
		m_Target->IsActive() ? Activate() : Deactivate();
	}
}