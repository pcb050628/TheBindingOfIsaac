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
		ImGui::Text("Select Atlas"); ImGui::SameLine();
		if (ImGui::Button("##Animator2DGUISelectTextureButton", ImVec2(20, 20)))
		{
			ListGUI* list = (ListGUI*)ImGuiManager::GetInst()->FindGUI("##ListGUI");
		
			std::vector<std::string> texlist;
			ResourceManager::GetInst()->GetAssetName(RESOURCE_TYPE::TEXTURE, texlist);
		
			list->AddString(texlist);
			list->SetDelegate(this, (Delegate_1)&AnimEditorGUI::SetAnimAtlas);
			list->Activate();
		}
		ImGui::SameLine();

		ImGui::Text("Select Anim"); ImGui::SameLine();
		if (ImGui::Button("##Animator2DGUISelectAnimButton", ImVec2(20, 20)))
		{
			ListGUI* list = (ListGUI*)ImGuiManager::GetInst()->FindGUI("##ListGUI");

			std::vector<std::string> animlist;
			ResourceManager::GetInst()->GetAssetName(RESOURCE_TYPE::ANIM, animlist);

			list->AddString(animlist);
			list->SetDelegate(this, (Delegate_1)&AnimEditorGUI::SetAnim);
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
			ImGui::Text("Duration"); ImGui::SameLine(); ImGui::InputFloat("##Animtor2DGUICreateDuration", &duration);
			m_EditAnim->SetDuration(duration);

			// 현재 프레임 리셋
			if (ImGui::Button("Reset##Animator2DGUIAllFrameResetButton"))
			{
				ResetCurFrame();
			}
			ImGui::SameLine();
			// 현재 프레임 삭제
			if (ImGui::Button("Remove Current Frame##Animator2DGUICreateRemoveCurFrame"))
			{
				if (!m_EditAnim->RemoveCurFrame())
				{
					curFrame = {};
				}
			}
			
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

		//모든 프레임 설정
		ImGui::Text("Set All Frame"); ImGui::Spacing();
		ImGui::Text("Slice      :"); ImGui::SameLine(); ImGui::DragFloat2("##Animtor2DGUIAllFrameSliceSizeSet", m_AllSliceSize);
		ImGui::Text("BackGround :"); ImGui::SameLine(); ImGui::DragFloat2("##Animtor2DGUIAllFrameBackgroundSizeSet", m_AllBackground);
		ImGui::Spacing();
		if (ImGui::Button("Apply##Animator2DGUIAllFrameSetApplyButton"))
		{
			SetAll();
		}

		m_EditAnim->LateUpdate();

		ImGui::Spacing();
		if (ImGui::Button("Save##Animtor2DGUICurAnimSaveButton"))
		{
			m_EditAnim->Save();
			ResourceManager::GetInst()->AddResource(m_EditAnim->GetResourceName(), m_EditAnim, true);
			Anim* an = new Anim(*m_EditAnim);
			m_EditAnim = an;
		}

		ImGui::EndTabItem();
	}

	active ? Activate() : Deactivate();
}

void AnimEditorGUI::SetAll()
{
	std::vector<Frame>& frm = m_EditAnim->GetAllFrame();
	for (int i = 0; i < frm.size(); i++)
	{
		if(0 != m_AllSliceSize.x && 0 != m_AllSliceSize.y)
			frm[i].vSliceSize = m_AllSliceSize;
		if (0 != m_AllBackground.x && 0 != m_AllBackground.y)
			frm[i].vBackground = m_AllBackground;
	}
}

void AnimEditorGUI::ResetCurFrame()
{
	Frame& curFrame = m_EditAnim->GetCurFrame();
	curFrame = {};
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

void AnimEditorGUI::SetAnim(DWORD_PTR _str)
{
	std::string animName = (char*)_str;
	Anim* anim = ResourceManager::GetInst()->Find<Anim>(ToWstring(animName));
	m_EditAnim = anim;
}

void AnimEditorGUI::Activate()
{
	GUI::Activate();
	if (m_RenderGUI)
		m_RenderGUI->Activate();
	if (nullptr == m_EditAnim)
	{
		CreateNewAnim();
	}
}

void AnimEditorGUI::Deactivate()
{
	GUI::Deactivate();
	DeleteAnim();
	if (m_RenderGUI)
		m_RenderGUI->Deactivate();
}
