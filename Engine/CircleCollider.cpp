#include "pch.h"
#include "CircleCollider.h"

CircleCollider::CircleCollider() : Collider(ColliderType::Circle)
	, m_Radius(1.0f)
{
}

CircleCollider::~CircleCollider()
{
}

void CircleCollider::Update()
{
}

void CircleCollider::LateUpdate()
{
}

