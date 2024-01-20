#include "pch.h"
#include "Animator2DGUI.h"

#include "ImGuiManager.h"
#include "InspectorGUI.h"
#include "ListGUI.h"

#include <Engine/ResourceManager.h>
#include <Engine/GameObject.h>
#include <Engine/Animator2D.h>
#include <Engine/Anim.h>
#include <Engine/Texture.h>

Animator2DGUI::Animator2DGUI() : ComponentGUI("Animator2D", "##Animator2DGUI", COMPONENT_TYPE::ANIMATOR2D)
{
	SetSize(ImVec2(0, 250));
}

Animator2DGUI::~Animator2DGUI()
{
}

void Animator2DGUI::RenderUpdate()
{
	ComponentGUI::RenderUpdate();
	{
		Animator2D* targetAnimator2D = GetTargetObject()->GetAnimator2D();
		std::vector<std::string> allAnim;
		targetAnimator2D->GetAllAnim(allAnim);
		Anim* curAnim = targetAnimator2D->GetCurAnim();

		if (curAnim)
		{
			std::string animName = ToString(curAnim->GetResourceName());
			ImGui::Text("Anim"); ImGui::SameLine(); ImGui::InputText("##Animtor2DGUIAnimName", (char*)animName.c_str(), animName.size(), ImGuiInputTextFlags_ReadOnly);
			ImGui::SameLine();
			
			if (ImGui::BeginCombo("##Animator2DGUIAnimSelectCombo", nullptr, ImGuiComboFlags_NoPreview))
			{
				for (int n = 0; n < allAnim.size(); n++)
				{
					const bool is_selected = (m_iSelectedIdx == n);
					if (ImGui::Selectable(allAnim[n].c_str(), is_selected))
					{
						m_iSelectedIdx = n;
						targetAnimator2D->Play(ToWstring(allAnim[n]));
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			ImGui::Spacing();

			if (ImGui::Button("AddAnim##Animator2DGUIAddAnimButton"))
			{
				ListGUI* list = (ListGUI*)ImGuiManager::GetInst()->FindGUI("##ListGUI");

				std::vector<std::string> animlist;
				ResourceManager::GetInst()->GetAssetName(RESOURCE_TYPE::ANIM, animlist);

				list->AddString(animlist);
				list->SetDelegate(this, (Delegate_1)&Animator2DGUI::AddAnim);
				list->Activate();
			}

			bool play = curAnim->IsPlaying();
			ImGui::Text("Play"); ImGui::SameLine(); ImGui::Checkbox("##Animtor2DGUIAnimPlayButton", &play);
			play ? curAnim->Play() : curAnim->Pause();

			ImGui::SameLine();

			bool repeat = curAnim->IsRepeat();
			ImGui::Text("Repeat"); ImGui::SameLine(); ImGui::Checkbox("##Animtor2DGUIAnimRepeatButton", &repeat);
			curAnim->SetRepeat(repeat);
		}
	}
}

void Animator2DGUI::AddAnim(DWORD_PTR _str)
{
	std::string name = (char*)_str;
	Anim* anim = ResourceManager::GetInst()->Find<Anim>(ToWstring(name));
	GetTargetObject()->GetAnimator2D()->AddAnim(anim);
	m_iSelectedIdx++;
}

