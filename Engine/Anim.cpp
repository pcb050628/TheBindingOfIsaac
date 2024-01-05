#include "pch.h"
#include "Anim.h"

#include "Device.h"
#include "ConstantBuffer.h"

#include "Time.h"

#include "Texture.h"

Anim::Anim() : Resource(RESOURCE_TYPE::ANIM)
	, m_Atlas(nullptr)
	, m_Frames{}
	, m_CurFrameIdx(0)
	, m_bIsPlaying(false)
	, m_bIsRepeat(false)
{
}

Anim::~Anim()
{
}

void Anim::UpdateData()
{
	ConstantBuffer* pCB = Device::GetInst()->GetConstBuffer(CB_TYPE::ANIMATION2D);

	g_AnimData.vLeftTop = m_Frames[m_CurFrameIdx].vLeftTop;
	g_AnimData.vSliceSize = m_Frames[m_CurFrameIdx].vSliceSize;
	g_AnimData.vOffset = m_Frames[m_CurFrameIdx].vOffset;
	g_AnimData.UseAnim2D = 1;
	pCB->SetData(&g_AnimData);
	pCB->UpdateData();
}

void Anim::LateUpdate()
{
	m_fAccTime += Time::GetInst()->GetDeltaTime();
	
	if (m_fAccTime > m_Frames[m_CurFrameIdx].fDuration)
	{
		m_CurFrameIdx++;
		m_fAccTime = 0;
		if (m_Frames.size() <= m_CurFrameIdx)
		{
			if (m_bIsRepeat)
				m_CurFrameIdx = 0;
			else
				--m_CurFrameIdx;
		}
	}
}

bool Anim::Load(const std::wstring& _strFilePath)
{
	return false;
}

bool Anim::Save()
{
	return false;
}

void Anim::Create(Texture* _atlas, Vec2 _leftTop, Vec2 _sliceSize, Vec2 _offset, Vec2 _background, int _frmCount, int _FPS)
{
	m_Atlas = _atlas;

	for (int i = 0; i < _frmCount; ++i)
	{
		Frame frm = {};

		frm.vSliceSize = Vec2(_sliceSize.x / (float)_atlas->GetWidth(), _sliceSize.y / (float)_atlas->GetHeight());

		frm.vLeftTop = Vec2(_leftTop.x / (float)_atlas->GetWidth() + frm.vSliceSize.x * i, _leftTop.y / (float)_atlas->GetHeight());

		frm.vOffset = Vec2(_offset.x / (float)_atlas->GetWidth(), _offset.y / (float)_atlas->GetHeight());
		frm.fDuration = 1.f / _FPS;

		frm.vBackground = Vec2(_background.x / (float)_atlas->GetWidth(), _background.y / (float)_atlas->GetHeight());


		m_Frames.push_back(frm);
	}
}
