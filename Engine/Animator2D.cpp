#include "pch.h"
#include "Animator2D.h"

#include "Anim.h"

#include "Device.h"
#include "ConstantBuffer.h"

Animator2D::Animator2D() : Component(COMPONENT_TYPE::ANIMATOR2D)
	, m_Anims{}
	, m_CurAnim(nullptr)
{
}

Animator2D::~Animator2D()
{
}

void Animator2D::Clear()
{
	ConstantBuffer* pCB = Device::GetInst()->GetConstBuffer(CB_TYPE::ANIMATION2D);
	g_AnimData.UseAnim2D = 0;

	pCB->SetData(&g_AnimData);
	pCB->UpdateData();
}

void Animator2D::UpdateData()
{
	if (m_CurAnim) m_CurAnim->UpdateData();
}

void Animator2D::LateUpdate()
{
	if (m_CurAnim) m_CurAnim->LateUpdate();
}

void Animator2D::AddAnim(Anim* _anim)
{
	m_Anims.insert(make_pair(_anim->GetResourceName(), _anim));
}

void Animator2D::Play(const std::wstring& _key, bool _repeat)
{
	auto iter = m_Anims.find(_key);
	if (iter == m_Anims.end())
		return;

	m_CurAnim = iter->second;
	m_CurAnim->Play();
	m_CurAnim->SetRepeat(_repeat);
}

void Animator2D::Play()
{
	if (m_CurAnim) m_CurAnim->Play();
}

void Animator2D::Pause()
{
	if (m_CurAnim) m_CurAnim->Pause();
}

void Animator2D::SetRepeat(bool _repeat)
{
	if (m_CurAnim) m_CurAnim->SetRepeat(_repeat);
}
