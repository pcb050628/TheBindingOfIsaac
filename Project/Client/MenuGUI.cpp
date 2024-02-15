#include "pch.h"
#include "MenuGUI.h"

#include "ImGuiManager.h"

#include "RoomEditorGUI.h"

MenuGUI::MenuGUI() : GUI("Menu", "##MenuGUI")
{
}

MenuGUI::~MenuGUI()
{
}

void MenuGUI::Render()
{
    if (ImGui::BeginMainMenuBar())
    {
        RenderUpdate();

        ImGui::EndMainMenuBar();
    }
}

void MenuGUI::RenderUpdate()
{
    //if (ImGui::BeginMenu("Menu"))
    //{
    //    ImGui::EndMenu();
    //}
    if (ImGui::BeginMenu("Edit"))
    {
        //if (ImGui::MenuItem("AnimEditor", nullptr))
        //{
        //    RoomEditorGUI* roomeditor = (RoomEditorGUI*)ImGuiManager::GetInst()->FindGUI("##RoomEditorGUI");
        //    roomeditor->Activate();
        //}

        if (ImGui::MenuItem("RoomEditor", nullptr))
        {
            RoomEditorGUI* roomeditor = (RoomEditorGUI*)ImGuiManager::GetInst()->FindGUI("##RoomEditorGUI");
            roomeditor->Activate();
        }

        //if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
        //if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
        //ImGui::Separator();
        //if (ImGui::MenuItem("Cut", "CTRL+X")) {}
        //if (ImGui::MenuItem("Copy", "CTRL+C")) {}
        //if (ImGui::MenuItem("Paste", "CTRL+V")) {}

        ImGui::EndMenu();
    }
}
