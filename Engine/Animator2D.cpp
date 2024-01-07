#include "pch.h"
#include "Animator2D.h"

#include "ResourceManager.h"

#include "Anim.h"

#include "Device.h"
#include "ConstantBuffer.h"

#include "Texture.h"

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

void Animator2D::CreateAnim(const wstring& _strKey, Texture* _altas
	, Vec2 _leftTop, Vec2 _sliceSize, Vec2 _offsetSize, Vec2 _background, int _frmCount, float _FPS)
{
	assert(!ResourceManager::GetInst()->IsExist(_strKey, RESOURCE_TYPE::ANIM));

	Anim* pAnim = new Anim;
	pAnim->SetResourceName(_strKey);
	pAnim->Create(_altas, _leftTop, _sliceSize, _offsetSize, _background, _frmCount, _FPS);
	m_Anims.insert(make_pair(_strKey, pAnim));
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

void Animator2D::Pause(bool _reset)
{
	if (m_CurAnim)
	{
		m_CurAnim->Pause(); 
		if (_reset) m_CurAnim->Reset();
	}
}

void Animator2D::SetRepeat(bool _repeat)
{
	if (m_CurAnim) m_CurAnim->SetRepeat(_repeat);
}
