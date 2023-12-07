#include "pch.h"
#include "BoxCollider.h"
#include "GameObject.h"

BoxCollider::BoxCollider() : Collider(ColliderType::Box)
	//, m_Scale(GetOwner()->GetScale())
{
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::Update()
{
}

void BoxCollider::LateUpdate()
{
}

