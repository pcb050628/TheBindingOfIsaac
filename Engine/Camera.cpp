#include "pch.h"
#include "Camera.h"

#include "GameObject.h"

#include "Transform.h"

Camera::Camera() : Component(COMPONENT_TYPE::CAMERA)
	, m_FOV(0.f)
	, m_Width(0.f)
	, m_Scale(0.f)
	, m_AspectRatio(1.f)
	, m_Far(10000.f)
{
}

Camera::~Camera()
{
}

void Camera::LateUpdate()
{
	m_matView = DirectX::XMMatrixIdentity();
	m_matProj = DirectX::XMMatrixIdentity();

	Vec3 vPos = GetOwner()->GetComponent<Transform>()->GetPos();

	m_matView = DirectX::XMMatrixTranslation(vPos.x, vPos.y, vPos.z);



	g_Transform.matView = m_matView;
	g_Transform.matProj = m_matProj;
}
