#include "pch.h"
#include "Anim.h"

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
}

void Anim::LateUpdate()
{
}

bool Anim::Load(const std::wstring& _strFilePath)
{
	return false;
}

bool Anim::Save()
{
	return false;
}
