#include "pch.h"
#include "Anim.h"

#include "Device.h"
#include "ConstantBuffer.h"

#include "Time.h"
#include "ResourceManager.h"

#include "Texture.h"

Anim::Anim() : Resource(RESOURCE_TYPE::ANIM)
	, m_Atlas(nullptr)
	, m_Frames{}
	, m_CurFrameIdx(0)
	, m_fAccTime(0.f)
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

	//10 번 레지스터가 anim atlas 용
	m_Atlas->UpdateData(10);
}

void Anim::LateUpdate()
{
	m_fAccTime += Time::GetInst()->GetDeltaTime();

	if (m_Frames[m_CurFrameIdx].fDuration < m_fAccTime)
	{
		m_fAccTime = 0.f;
		m_CurFrameIdx++;
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
