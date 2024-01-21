#include "pch.h"
#include "CameraGUI.h"

#include <Engine/GameObject.h>
#include <Engine/Camera.h>

CameraGUI::CameraGUI() : ComponentGUI("Camera", "##CaemraGUI", COMPONENT_TYPE::CAMERA)
    , m_SelectInt(-1)
{
    SetSize(ImVec2(0, 200));
    m_ProjString.push_back("ORTHOGRAPHICS");
    m_ProjString.push_back("PERSPECTIVE");
}

CameraGUI::~CameraGUI()
{
}

void CameraGUI::RenderUpdate()
{
    if (nullptr == GetTargetObject())
        return;

	ComponentGUI::RenderUpdate();

    Camera* targetCamera = GetTargetObject()->GetCamera();

    //projection
    m_SelectInt = (UINT)targetCamera->GetProjType();
    ImGui::Text("Projection"); ImGui::SameLine();
    if (ImGui::BeginCombo("##CameraProjTypeCombo", nullptr, ImGuiComboFlags_NoPreview))
    {
        for (int n = 0; n < m_ProjString.size(); n++)
        {
            const bool is_selected = (m_SelectInt == n);
            if (ImGui::Selectable(m_ProjString[n].c_str(), is_selected))
                m_SelectInt = n;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    targetCamera->SetProjType((PROJ_TYPE)m_SelectInt);

    //far
    float cameraFar = targetCamera->GetFar();
    ImGui::Text("Far"); ImGui::SameLine(); ImGui::DragFloat("##CameraGUIFar", &cameraFar, 0.1f, 100.f);
    if (cameraFar < 100.f)
        cameraFar = 100.f;
    targetCamera->SetFar(cameraFar);
     
    //// perspective
    ImGui::Text("Perspective");
    //fov
    float fov = targetCamera->GetFOV();
    ImGui::Text("Fov"); ImGui::SameLine(); ImGui::DragFloat("##CameraGUIFov", &fov, 0.1f);
    targetCamera->SetFOV(fov);
    //// orthographic
    ImGui::Text("Orthographic");
    //width
    //float width = targetCamera->GetWidth();
    //ImGui::Text("Width"); ImGui::SameLine(); ImGui::DragFloat("##CameraGUIWidth", &width, 0.1f);
    //targetCamera->SetWidth(width);
    //scale
    float scale = targetCamera->GetScale();
    ImGui::Text("Scale"); ImGui::SameLine(); ImGui::DragFloat("##CameraGUIScale", &scale, 0.1f);
    targetCamera->SetScale(scale);
    
    //UINT    m_LayerCheck; // treenode, checkBox

    if (ImGui::TreeNode("Layer Check##CameraGUILayerCheck"))
    {
        for (int i = 0; i < (UINT)LAYER_TYPE::END; i++)
        {
            char buf[50];
            _itoa_s(i, buf, 10);
            std::string label = "##CameraGUILayer";
            label += buf;
            bool check = targetCamera->GetLayerCheck(i);
            ImGui::Text(GetLayerName((LAYER_TYPE)i).c_str()); ImGui::SameLine(); ImGui::Checkbox(label.c_str(), &check);
            targetCamera->LayerCheck(i, check);
        }

        ImGui::TreePop();
    }
}

std::string CameraGUI::GetLayerName(LAYER_TYPE _layer)
{
    std::string out;

    switch (_layer)
    {
    case LAYER_TYPE::Background:
        out = "Background";
        break;
    case LAYER_TYPE::Light:
        out = "Light";
        break;
    case LAYER_TYPE::Object:
        out = "Object";
        break;
    case LAYER_TYPE::Monster:
        out = "Monster";
        break;
    case LAYER_TYPE::Player:
        out = "Player";
        break;
    case LAYER_TYPE::Pickup:
        out = "Pickup";
        break;
    case LAYER_TYPE::Item:
        out = "Item";
        break;
    case LAYER_TYPE::UI:
        out = "UI";
        break;
    case LAYER_TYPE::Camera:
        out = "Camera";
        break;
    }

    return out;
}
