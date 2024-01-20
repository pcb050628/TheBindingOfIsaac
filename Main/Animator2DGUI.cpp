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

void CreatAnimTextrue(DWORD_PTR _str);
void ChangeAnim(DWORD_PTR _str);


Animator2DGUI::Animator2DGUI() : ComponentGUI("Animator2D", "##Animator2DGUI", COMPONENT_TYPE::ANIMATOR2D)
	, m_AdditionalAnim(nullptr)
	, m_bCreateMod(false)
	, m_iNameInitSize(30)
{
	SetSize(ImVec2(0, 250));
}

Animator2DGUI::~Animator2DGUI()
{
	if (m_AdditionalAnim)
		delete m_AdditionalAnim;
}

void Animator2DGUI::RenderUpdate()
{
	ComponentGUI::RenderUpdate();
	{
		Animator2D* targetAnimator2D = GetTargetObject()->GetAnimator2D();
		Anim* curAnim = targetAnimator2D->GetCurAnim();
		Frame& curFrame = curAnim->GetCurFrame();

		if (curAnim)
		{
			std::string animName = ToString(curAnim->GetResourceName());
			ImGui::Text("Anim"); ImGui::SameLine(); ImGui::InputText("##Animtor2DGUIAnimName", (char*)animName.c_str(), animName.size(), ImGuiInputTextFlags_ReadOnly);
			ImGui::SameLine();
			

			if (ImGui::Button("AddAtlas##Animator2DGUIAddAnimButton"))
			{
				ListGUI* list = (ListGUI*)ImGuiManager::GetInst()->FindGUI("##ListGUI");

				std::vector<std::string> animlist;
				ResourceManager::GetInst()->GetAssetName(RESOURCE_TYPE::ANIM, animlist);

				list->AddString(animlist);
				list->SetCallBack(ChangeAnim);
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

void Animator2DGUI::SetCreateAnimTex(Texture* _tex)
{
	m_AdditionalAnim->SetAtlas(_tex);
}

void Animator2DGUI::CreateAdditionalAnim()
{
	Animator2D* animator = GetTargetObject()->GetAnimator2D();

	m_ExistingAnim = animator->GetCurAnim()->GetResourceName();

	if (m_AdditionalAnim)
	{
		delete m_AdditionalAnim;
		m_AdditionalAnim = nullptr;
	}

	m_AdditionalAnim = new Anim;
	m_AdditionalAnim->SetResourceName(L"New");
	m_AdditionalAnim->GetResourceName().resize(m_iNameInitSize);

	m_AdditionalAnim->CreateNewFrame();
	animator->AddAnim(m_AdditionalAnim);
	animator->Play(m_AdditionalAnim->GetResourceName());
	m_AdditionalAnim->Pause();
}

void Animator2DGUI::DeleteAdditionalAnim()
{
	Animator2D* animator = GetTargetObject()->GetAnimator2D();

	animator->RemoveAnim(m_AdditionalAnim->GetResourceName());
	delete m_AdditionalAnim;
	m_AdditionalAnim = nullptr;
	GetTargetObject()->GetAnimator2D()->Play(m_ExistingAnim);
}

void CreatAnimTextrue(DWORD_PTR _str)
{
	std::wstring texName = ToWstring((char*)_str);
	Texture* pTex = ResourceManager::GetInst()->Find<Texture>(texName);
	assert(pTex);
	InspectorGUI* inspector = (InspectorGUI*)ImGuiManager::GetInst()->FindGUI("##InspectorGUI");
	Animator2D* animator = inspector->GetTargetObject()->GetAnimator2D();
	Animator2DGUI* animatorGUI = (Animator2DGUI*)inspector->GetComponentGUI(COMPONENT_TYPE::ANIMATOR2D);
	animatorGUI->SetCreateAnimTex(pTex);
}


void ChangeAnim(DWORD_PTR _str)
{
	std::wstring animName = ToWstring((char*)_str);
	Anim* pAnim = ResourceManager::GetInst()->Find<Anim>(animName);
	assert(pAnim);
	InspectorGUI* inspector = (InspectorGUI*)ImGuiManager::GetInst()->FindGUI("##InspectorGUI");
	Animator2D* animator = inspector->GetTargetObject()->GetAnimator2D();
	animator->AddAnim(pAnim);
}
