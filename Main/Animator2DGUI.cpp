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

Animator2DGUI::Animator2DGUI() : ComponentGUI("Animator2D", "##Animator2DGUI", COMPONENT_TYPE::ANIMATOR2D)
	, m_bCreateMod(false)
	, m_CreateAnimTex(nullptr)
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
		Anim* curAnim = targetAnimator2D->GetCurAnim();
		Frame& curFrame = curAnim->GetCurFrame();

		ImGui::Checkbox("CreateMode##Animtor2DGUIAnimCreateButton", &m_bCreateMod);

		if (m_bCreateMod)
		{
			// 아틀라스 고르기
			if (ImGui::Button("SelectAtlas##Animator2DGUISelectTextureButton"))
			{
				ListGUI* list = (ListGUI*)ImGuiManager::GetInst()->FindGUI("##ListGUI");

				std::vector<std::string> texlist;
				ResourceManager::GetInst()->GetAssetName(RESOURCE_TYPE::TEXTURE, texlist);

				list->AddString(texlist);
				list->SetCallBack(CreatAnimTextrue);
				list->Activate();
			}

			std::string atlasName = "";
			if (m_CreateAnimTex)
			{
				atlasName = ToString(m_CreateAnimTex->GetResourceName());
			}
			ImGui::Text("Atlas"); ImGui::SameLine(); ImGui::InputText("##Animator2DGUICreateAtlas", (char*)atlasName.c_str(), atlasName.size(), ImGuiInputTextFlags_ReadOnly);
			ImGui::Spacing();
			
			// 최대 프레임 수, 현재 프레임 따로 띄우기
			
			
			// Duration 설정
			
			// 현재 프레임 설정
			// frmBackground
			// frmLeftTop
			// frmOffset
			// frmSliceSize

			if (ImGui::Button("Save##Animtor2DGUICurAnimSaveButton"))
				curAnim->Save();
		}
		else
		{
			if (curAnim)
			{
				ImGui::Text(ToString(curAnim->GetResourceName()).c_str());
				std::string atlasName = ToString(curAnim->GetAtlas()->GetResourceName());
				ImGui::Text("Atlas"); ImGui::SameLine(); ImGui::InputText("##Animtor2DGUIAtlas", (char*)atlasName.c_str(), atlasName.size(), ImGuiInputTextFlags_ReadOnly);

				bool play = curAnim->IsPlaying();
				ImGui::Text("Play"); ImGui::SameLine(); ImGui::Checkbox("##Animtor2DGUIAnimRepeat", &play);
				play ? curAnim->Play() : curAnim->Pause();

				int curfrmIdx = curAnim->GetCurFrameIdx();
				ImGui::Text("FrameIdx"); ImGui::SameLine(); ImGui::InputInt("##Animtor2DGUIAnimFrameIdx", &curfrmIdx);
				curAnim->SetCurFrameIdx(curfrmIdx);

				float duration = curAnim->GetDuration();
				ImGui::Text("Duration"); ImGui::SameLine(); ImGui::InputFloat("##Animtor2DGUIAnimDuration", &duration);
				curAnim->SetDuration(duration);
				
				ImGui::Spacing();

				Vec2 frmBackground	= Vec2(curFrame.vBackground.x, curFrame.vBackground.y);
				Vec2 frmLeftTop		= Vec2(curFrame.vLeftTop.x, curFrame.vLeftTop.y);
				Vec2 frmOffset		= Vec2(curFrame.vOffset.x, curFrame.vOffset.y);
				Vec2 frmSliceSize	= Vec2(curFrame.vSliceSize.x, curFrame.vSliceSize.y);

				ImGui::Text("BackGround :"); ImGui::SameLine(); ImGui::DragFloat2("##Animtor2DGUIFrameBackgroundSize"	, frmBackground);
				ImGui::Text("LeftTop    :"); ImGui::SameLine(); ImGui::DragFloat2("##Animtor2DGUIFrameLeftTopSize"		, frmLeftTop);
				ImGui::Text("Offset     :"); ImGui::SameLine(); ImGui::DragFloat2("##Animtor2DGUIFrameOffsetSize"		, frmOffset);
				ImGui::Text("Slice      :"); ImGui::SameLine(); ImGui::DragFloat2("##Animtor2DGUIFrameSliceSize"		, frmSliceSize);

				curFrame.vBackground = frmBackground;
				curFrame.vLeftTop = frmLeftTop;
				curFrame.vOffset = frmOffset;
				curFrame.vSliceSize = frmSliceSize;
			}
		}
	}
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