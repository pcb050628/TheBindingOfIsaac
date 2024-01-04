#include "pch.h"
#include "Animator.h"

#include "Actor.h"

Animator::Animator() : Super(ComponentType::ANIMATOR)
	, m_CurAnim(nullptr)
	, m_Anims({})
{
}

Animator::~Animator()
{
}

void Animator::Update()
{
	
}

void Animator::LateUpdate()
{
	if (m_CurAnim != nullptr)
		m_CurAnim->LateUpdate();
}

void Animator::Render()
{
	if (m_CurAnim != nullptr)
	{
		m_CurAnim->Render(GetOwner()->GetPosition());
	}
}