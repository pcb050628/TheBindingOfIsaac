#include "pch.h"
#include "Collider.h"
#include "Actor.h"

Collider::Collider(ColliderType _type) : Component(ComponentType::COLLIDER)
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
	return m_PosOffset + GetOwner()->GetPosition();
}

void Collider::BeginOverlap(Collider* _myCol, Actor* _otherActor, Collider* _otherCol)
{
}

void Collider::Overlap(Collider* _myCol, Actor* _otherActor, Collider* _otherCol)
{
}

void Collider::EndOverlap(Collider* _myCol, Actor* _otherActor, Collider* _otherCol)
{
}
