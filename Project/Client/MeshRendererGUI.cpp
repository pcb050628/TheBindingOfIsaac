#include "pch.h"
#include "MeshRendererGUI.h"

#include "ImGuiManager.h"
#include "InspectorGUI.h"
#include "ListGUI.h"

#include <Engine/ResourceManager.h>

#include <Engine/GameObject.h>

#include <Engine/MeshRenderer.h>
#include <Engine/Material.h>
#include <Engine/Mesh.h>

void ChangeMesh(DWORD_PTR _str);
void ChangeMtrl(DWORD_PTR _str);

MeshRendererGUI::MeshRendererGUI() : ComponentGUI("MeshRenderer", "##MeshRendererGUI", COMPONENT_TYPE::MESHRENDERER)
{
	SetSize(ImVec2(0, 80));
}

MeshRendererGUI::~MeshRendererGUI()
{
}

void MeshRendererGUI::RenderUpdate()
{
	if (nullptr == GetTargetObject())
		return;

	ComponentGUI::RenderUpdate();

	MeshRenderer* target = GetTargetObject()->GetMeshRenderer();
	Mesh* mesh = target->GetMesh();
	Material* mtrl = target->GetMaterial();

	if (nullptr != mesh)
	{
		std::string meshName = ToString(mesh->GetResourceName());
		ImGui::Text("Mesh    "); ImGui::SameLine(); ImGui::InputText("##MeshRendererGUIMesh", (char*)meshName.c_str(), meshName.size(), ImGuiInputTextFlags_ReadOnly);
	}
	else
	{
		ImGui::Text("Mesh    ");
	}

	if (nullptr != mtrl)
	{
		std::string mtrlName = ToString(mtrl->GetResourceName());
		ImGui::Text("Material"); ImGui::SameLine(); ImGui::InputText("##MeshRendererGUIMaterial", (char*)mtrlName.c_str(), mtrlName.size(), ImGuiInputTextFlags_ReadOnly);
	}
	else
	{
		ImGui::Text("Material");
	}
	ImGui::SameLine();

	if (ImGui::Button("SelectMesh##MeshRendererGUISelectMeshButton"))
	{
		ListGUI* list = (ListGUI*)ImGuiManager::GetInst()->FindGUI("##ListGUI");

		std::vector<std::string> meshlist;
		ResourceManager::GetInst()->GetAssetName(RESOURCE_TYPE::MESH, meshlist);

		list->AddString(meshlist);
		list->SetCallBack(ChangeMesh);
		list->Activate();
	}
	
	ImGui::SameLine();
	if (ImGui::Button("SelectMtrl##MeshRendererGUISelectMaterialButton"))
	{
		ListGUI* list = (ListGUI*)ImGuiManager::GetInst()->FindGUI("##ListGUI");

		std::vector<std::string> mtrllist;
		ResourceManager::GetInst()->GetAssetName(RESOURCE_TYPE::MATERIAL, mtrllist);

		list->AddString(mtrllist);
		list->SetCallBack(ChangeMtrl);
		list->Activate();
	}
}

void ChangeMesh(DWORD_PTR _str)
{
	std::wstring meshName = ToWstring((char*)_str);
 	Mesh* pMesh = ResourceManager::GetInst()->Find<Mesh>(meshName);
	assert(pMesh);
	InspectorGUI* inspector = (InspectorGUI*)ImGuiManager::GetInst()->FindGUI("##InspectorGUI");
	MeshRenderer* mr = inspector->GetTargetObject()->GetMeshRenderer();
	mr->SetMesh(pMesh);
}

void ChangeMtrl(DWORD_PTR _str)
{
	std::wstring meshName = ToWstring((char*)_str);
	Material* pMtrl = ResourceManager::GetInst()->Find<Material>(meshName);
	assert(pMtrl);
	InspectorGUI* inspector = (InspectorGUI*)ImGuiManager::GetInst()->FindGUI("##InspectorGUI");
	MeshRenderer* mr = inspector->GetTargetObject()->GetMeshRenderer();
	mr->SetMaterial(pMtrl);
}
