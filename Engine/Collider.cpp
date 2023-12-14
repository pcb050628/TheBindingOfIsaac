#include "pch.h"
#include "Collider.h"
#include "GameObject.h"

Collider::Collider(ColliderType _type) : Component(COMPONENT_TYPE::COLLIDER2D)
	, m_Type(_type)
	, m_PosOffset()
{
}

Collider::~Collider()
{
}

void Collider::Update()
{
}

void Collider::LateUpdate()
{
}

Vec2 Collider::GetFinalPos()
{
	//return m_PosOffset + GetOwner()->GetPosition();
	return m_PosOffset + Vec2();
}

void Collider::BeginOverlap(Collider* _myCol, GameObject* _otherActor, Collider* _otherCol)
{
}

void Collider::Overlap(Collider* _myCol, GameObject* _otherActor, Collider* _otherCol)
{
}

void Collider::EndOverlap(Collider* _myCol, GameObject* _otherActor, Collider* _otherCol)
{
}
