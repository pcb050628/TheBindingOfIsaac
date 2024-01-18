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

		bool createMod = m_bCreateMod;
		ImGui::Checkbox("CreateMode##Animtor2DGUIAnimCreateButton", &m_bCreateMod);

		if (createMod != m_bCreateMod)
		{
			if (m_bCreateMod)
				CreateAdditionalAnim();
			else
				DeleteAdditionalAnim();
		}

		if (m_bCreateMod)
		{
			Frame& curFrame = m_AdditionalAnim->GetCurFrame();

			std::string name = ToString(m_AdditionalAnim->GetResourceName());
			const char* buf = name.data();//m_AnimName.data();
			ImGui::InputText("##Animatro2DGUICreateSetAnimName", (char*)buf, m_iNameInitSize);
			m_AdditionalAnim->SetResourceName(ToWstring(string(buf)));

			bool play = m_AdditionalAnim->IsPlaying();
			ImGui::Text("Play"); ImGui::SameLine(); ImGui::Checkbox("##Animtor2DGUIAnimCreatePlayButton", &play);
			play ? m_AdditionalAnim->Play() : m_AdditionalAnim->Pause();

			bool repeat = m_AdditionalAnim->IsRepeat();
			ImGui::Text("Repeat"); ImGui::SameLine(); ImGui::Checkbox("##Animtor2DGUIAnimCreateRepeatButton", &repeat);
			m_AdditionalAnim->SetRepeat(repeat);

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
			if (m_AdditionalAnim->GetAtlas())
			{
				atlasName = ToString(m_AdditionalAnim->GetAtlas()->GetResourceName());
			}
			ImGui::Text("Atlas"); ImGui::SameLine(); ImGui::InputText("##Animator2DGUICreateAtlas", (char*)atlasName.c_str(), atlasName.size(), ImGuiInputTextFlags_ReadOnly);
			ImGui::Spacing();
			
			int maxFrameIdx = m_AdditionalAnim->GetMaxFrameIdx();
			int curFrameIdx = m_AdditionalAnim->GetCurFrameIdx();
			float duration  = m_AdditionalAnim->GetDuration();

			// 최대 프레임 수, 현재 프레임 따로 띄우기
			ImGui::Text("MaxFrame"); ImGui::SameLine(); ImGui::InputInt("##Animtor2DGUICreateMaxFrameIdx", &maxFrameIdx);
			int maxi = m_AdditionalAnim->GetMaxFrameIdx();
			if (maxFrameIdx > maxi)
			{
				int n = maxFrameIdx - maxi;
				for (int i = 0; i < n; i++)
				{
					m_AdditionalAnim->CreateNewFrame();
				}
			}

			ImGui::Text("CurFrame"); ImGui::SameLine(); ImGui::InputInt("##Animtor2DGUICreateCurFrameIdx", &curFrameIdx);
			m_AdditionalAnim->SetCurFrameIdx(curFrameIdx);

			// Duration 설정
			ImGui::Text("CurDuration"); ImGui::SameLine(); ImGui::InputFloat("##Animtor2DGUICreateDuration", &duration);
			m_AdditionalAnim->SetDuration(duration);
			
			// 현재 프레임 삭제
			if (ImGui::Button("Remove Current Frame##Animator2DGUICreateRemoveCurFrame"))
				m_AdditionalAnim->RemoveCurFrame();

			ImGui::Spacing();

			// 현재 프레임 설정
			Vec2 frmBackground = Vec2(curFrame.vBackground.x, curFrame.vBackground.y);
			Vec2 frmLeftTop = Vec2(curFrame.vLeftTop.x, curFrame.vLeftTop.y);
			Vec2 frmOffset = Vec2(curFrame.vOffset.x, curFrame.vOffset.y);
			Vec2 frmSliceSize = Vec2(curFrame.vSliceSize.x, curFrame.vSliceSize.y);

			ImGui::Text("BackGround :"); ImGui::SameLine(); ImGui::DragFloat2("##Animtor2DGUIFrameBackgroundSize", frmBackground);
			ImGui::Text("LeftTop    :"); ImGui::SameLine(); ImGui::DragFloat2("##Animtor2DGUIFrameLeftTopSize", frmLeftTop);
			ImGui::Text("Offset     :"); ImGui::SameLine(); ImGui::DragFloat2("##Animtor2DGUIFrameOffsetSize", frmOffset);
			ImGui::Text("Slice      :"); ImGui::SameLine(); ImGui::DragFloat2("##Animtor2DGUIFrameSliceSize", frmSliceSize);

			curFrame.vBackground = frmBackground;
			curFrame.vLeftTop = frmLeftTop;
			curFrame.vOffset = frmOffset;
			curFrame.vSliceSize = frmSliceSize;

			if (ImGui::Button("Save##Animtor2DGUICurAnimSaveButton"))
			{
				m_AdditionalAnim->Save();
			}

			if (ImGui::Button("Save##Animtor2DGUICurAnimSaveButton"))
			{
				targetAnimator2D->AddAnim(m_AdditionalAnim);
				m_AdditionalAnim = nullptr;
			}
		}
		else
		{
			Anim* curAnim = targetAnimator2D->GetCurAnim();
			Frame& curFrame = curAnim->GetCurFrame();

			if (curAnim)
			{
				std::string atlasName = ToString(curAnim->GetResourceName());
				ImGui::Text("Atlas"); ImGui::SameLine(); ImGui::InputText("##Animtor2DGUIAtlasName", (char*)atlasName.c_str(), atlasName.size(), ImGuiInputTextFlags_ReadOnly);
				ImGui::SameLine();
				if (ImGui::Button("SelectAtlas##Animator2DGUISelectAnimButton"))
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

				bool repeat = curAnim->IsRepeat();
				ImGui::Text("Repeat"); ImGui::SameLine(); ImGui::Checkbox("##Animtor2DGUIAnimRepeatButton", &repeat);
				curAnim->SetRepeat(repeat);

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
