#include "pch.h"
#include "AnimEditorGUI.h"

#include "ImGuiManager.h"
#include "AnimEditorRenderGUI.h"
#include "ListGUI.h"

#include <Engine/ResourceManager.h>
#include <Engine/Anim.h>
#include <Engine/Texture.h>

AnimEditorGUI::AnimEditorGUI() : GUI("AnimEditor", "##AnimEditorGUI")
	, m_EditAnim(nullptr)
	, m_RenderGUI(nullptr)
{
	Activate();
}

AnimEditorGUI::~AnimEditorGUI()
{
}

void AnimEditorGUI::RenderUpdate()
{
	bool active = IsActive();
	if (active && ImGui::BeginTabItem("AnimEditor##AnimEditorGUITabBar", &active))
	{
		std::string name = ToString(m_EditAnim->GetResourceName());
		const char* buf = name.data();
		if (ImGui::InputText("##Animatro2DGUICreateSetAnimName", (char*)buf, 50))
		{
			int a = 0;
		}
		m_EditAnim->SetResourceName(ToWstring(string(buf)));

		// 아틀라스 고르기
		if (ImGui::Button("SelectAtlas##Animator2DGUISelectTextureButton"))
		{
			ListGUI* list = (ListGUI*)ImGuiManager::GetInst()->FindGUI("##ListGUI");
		
			std::vector<std::string> texlist;
			ResourceManager::GetInst()->GetAssetName(RESOURCE_TYPE::TEXTURE, texlist);
		
			list->AddString(texlist);
			list->SetDelegate(this, (Delegate_1)&AnimEditorGUI::SetAnimAtlas);
			list->Activate();
		}
		
		if (m_EditAnim->GetAtlas())
		{
			if (!m_RenderGUI)
			{
				m_RenderGUI = (AnimEditorRenderGUI*)ImGuiManager::GetInst()->FindGUI("##AnimEditorRenderGUI");
			}

			if (!m_RenderGUI->IsActive())
			{
				m_RenderGUI->Activate();
				m_RenderGUI->SetTarget(this);
			}

			Frame& curFrame = m_EditAnim->GetCurFrame();

			bool play = m_EditAnim->IsPlaying();
			ImGui::Text("Play"); ImGui::SameLine(); ImGui::Checkbox("##Animtor2DGUIAnimCreatePlayButton", &play);
			play ? m_EditAnim->Play() : m_EditAnim->Pause();

			bool repeat = m_EditAnim->IsRepeat();
			ImGui::Text("Repeat"); ImGui::SameLine(); ImGui::Checkbox("##Animtor2DGUIAnimCreateRepeatButton", &repeat);
			m_EditAnim->SetRepeat(repeat);

			std::string atlasName = ToString(m_EditAnim->GetAtlas()->GetResourceName());
			ImGui::Text("Atlas"); ImGui::SameLine(); ImGui::InputText("##Animator2DGUICreateAtlas", (char*)atlasName.c_str(), atlasName.size(), ImGuiInputTextFlags_ReadOnly);
			ImGui::Spacing();
			
			int maxFrameIdx = m_EditAnim->GetMaxFrameIdx();
			int curFrameIdx = m_EditAnim->GetCurFrameIdx();
			float duration = m_EditAnim->GetDuration();
			
			// 최대 프레임 수, 현재 프레임 따로 띄우기
			ImGui::Text("MaxFrame"); ImGui::SameLine(); ImGui::InputInt("##Animtor2DGUICreateMaxFrameIdx", &maxFrameIdx);
			int maxi = m_EditAnim->GetMaxFrameIdx();
			if (maxFrameIdx > maxi)
			{
				int n = maxFrameIdx - maxi;
				for (int i = 0; i < n; i++)
				{
					m_EditAnim->CreateNewFrame();
				}
			}
			
			ImGui::Text("CurFrame"); ImGui::SameLine(); ImGui::InputInt("##Animtor2DGUICreateCurFrameIdx", &curFrameIdx);
			m_EditAnim->SetCurFrameIdx(curFrameIdx);
			
			// Duration 설정
			ImGui::Text("CurDuration"); ImGui::SameLine(); ImGui::InputFloat("##Animtor2DGUICreateDuration", &duration);
			m_EditAnim->SetDuration(duration);
			
			// 현재 프레임 삭제
			if (ImGui::Button("Remove Current Frame##Animator2DGUICreateRemoveCurFrame"))
				m_EditAnim->RemoveCurFrame();
			
			ImGui::Spacing();
			
			// 현재 프레임 설정
			Vec2 frmLeftTop		= Vec2(curFrame.vLeftTop.x	, curFrame.vLeftTop.y);
			Vec2 frmSliceSize	= Vec2(curFrame.vSliceSize.x, curFrame.vSliceSize.y);
			Vec2 frmOffset		= Vec2(curFrame.vOffset.x	, curFrame.vOffset.y);
			Vec2 frmBackground = Vec2(curFrame.vBackground.x, curFrame.vBackground.y);
			
			ImGui::Text("LeftTop    :"); ImGui::SameLine(); ImGui::DragFloat2("##Animtor2DGUIFrameLeftTopSize", frmLeftTop);
			ImGui::Text("Slice      :"); ImGui::SameLine(); ImGui::DragFloat2("##Animtor2DGUIFrameSliceSize", frmSliceSize);
			ImGui::Text("Offset     :"); ImGui::SameLine(); ImGui::DragFloat2("##Animtor2DGUIFrameOffsetSize", frmOffset);
			ImGui::Text("BackGround :"); ImGui::SameLine(); ImGui::DragFloat2("##Animtor2DGUIFrameBackgroundSize", frmBackground);
			
			curFrame.vLeftTop	 = frmLeftTop;
			curFrame.vSliceSize  = frmSliceSize;
			curFrame.vOffset	 = frmOffset;
			curFrame.vBackground = frmBackground;

			if (curFrame.vBackground.x < curFrame.vSliceSize.x)
			{
				curFrame.vBackground.x = curFrame.vSliceSize.x;
			}
			if (curFrame.vBackground.y < curFrame.vSliceSize.y)
			{
				curFrame.vBackground.y = curFrame.vSliceSize.y;
			}
		}

		m_EditAnim->LateUpdate();
		
		if (ImGui::Button("Save##Animtor2DGUICurAnimSaveButton"))
		{
			m_EditAnim->Save();
		}

		ImGui::EndTabItem();
	}

	active ? Activate() : Deactivate();
}

void AnimEditorGUI::CreateNewAnim()
{
	if (m_EditAnim)
		delete m_EditAnim;

	m_EditAnim = new Anim;
	m_EditAnim->CreateNewFrame();
	m_EditAnim->SetResourceName(L"New");
	m_EditAnim->GetResourceName().resize(50);
}

void AnimEditorGUI::DeleteAnim()
{
	if (m_EditAnim)
	{
		delete m_EditAnim;
		m_EditAnim = nullptr;
	}
}

void AnimEditorGUI::SetAnimAtlas(DWORD_PTR _str)
{
	std::string texName = (char*)_str;
	Texture* pTex = ResourceManager::GetInst()->Find<Texture>(ToWstring(texName));
	m_EditAnim->SetAtlas(pTex);
}

void AnimEditorGUI::Activate()
{
	GUI::Activate();
	if (nullptr == m_EditAnim)
	{
		CreateNewAnim();
	}
}

void AnimEditorGUI::Deactivate()
{
	GUI::Deactivate();
	DeleteAnim();
}
