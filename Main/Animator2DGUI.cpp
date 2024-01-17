#include "pch.h"
#include "Animator2DGUI.h"

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
		Anim* curAnim = targetAnimator2D->GetCurAnim();
		Frame& curFrame = curAnim->GetCurFrame();

		ImGui::Text(ToString(curAnim->GetResourceName()).c_str());
		ImGui::Text("Atlas Name :"); ImGui::SameLine(); ImGui::Text(ToString(curAnim->GetAtlas()->GetResourceName()).c_str());

		bool play = curAnim->IsPlaying();
		ImGui::Text("Play"); ImGui::SameLine(); ImGui::Checkbox("##CurAnimRepeat", &play);
		play ? curAnim->Play() : curAnim->Pause();

		int curfrmIdx = curAnim->GetCurFrameIdx();
		ImGui::Text("FrameIdx"); ImGui::SameLine(); ImGui::InputInt("##CurAnimFrameIdx", &curfrmIdx);
		curAnim->SetCurFrameIdx(curfrmIdx);

		float duration = curAnim->GetDuration();
		ImGui::Text("Duration"); ImGui::SameLine(); ImGui::InputFloat("##CurAnimDuration", &duration);
		curAnim->SetDuration(duration);
		
		ImGui::Spacing();

		Vec2 frmBackground	= Vec2(curFrame.vBackground.x, curFrame.vBackground.y);
		Vec2 frmLeftTop		= Vec2(curFrame.vLeftTop.x, curFrame.vLeftTop.y);
		Vec2 frmOffset		= Vec2(curFrame.vOffset.x, curFrame.vOffset.y);
		Vec2 frmSliceSize	= Vec2(curFrame.vSliceSize.x, curFrame.vSliceSize.y);

		ImGui::Text("BackGround :"); ImGui::SameLine(); ImGui::DragFloat2("##FrameBackgroundSize"	, frmBackground);
		ImGui::Text("LeftTop    :"); ImGui::SameLine(); ImGui::DragFloat2("##FrameLeftTopSize"		, frmLeftTop);
		ImGui::Text("Offset     :"); ImGui::SameLine(); ImGui::DragFloat2("##FrameOffsetSize"		, frmOffset);
		ImGui::Text("Slice      :"); ImGui::SameLine(); ImGui::DragFloat2("##FrameSliceSize"		, frmSliceSize);

		curFrame.vBackground = frmBackground;
		curFrame.vLeftTop = frmLeftTop;
		curFrame.vOffset = frmOffset;
		curFrame.vSliceSize = frmSliceSize;

		if (ImGui::Button("Save##CurAnimSaveButton"))
			curAnim->Save();
	}
}
