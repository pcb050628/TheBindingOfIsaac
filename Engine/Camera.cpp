#include "pch.h"
#include "Camera.h"

#include "Device.h"

#include "GameObject.h"

#include "Transform.h"

Camera::Camera() : Component(COMPONENT_TYPE::CAMERA)\
	, m_ProjType(PROJ_TYPE::ORTHOGRAPHICS)
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

	{ // view matrix
		// 위치 행렬
		Vec3 vPos = GetOwner()->GetComponent<Transform>()->GetPos();
		Matrix matPos = DirectX::XMMatrixTranslation(-vPos.x, -vPos.y, -vPos.z);

		Vec3 vRight = GetOwner()->GetComponent<Transform>()->GetDir(DIR_TYPE::RIGHT);
		Vec3 vUp = GetOwner()->GetComponent<Transform>()->GetDir(DIR_TYPE::UP);
		Vec3 vFront = GetOwner()->GetComponent<Transform>()->GetDir(DIR_TYPE::FRONT);

		// 회전 행렬
		Matrix matRot = DirectX::XMMatrixIdentity();

		matRot._11 = vRight.x; matRot._12 = vUp.x; matRot._13 = vFront.x;
		matRot._21 = vRight.y; matRot._22 = vUp.y; matRot._23 = vFront.y;
		matRot._31 = vRight.z; matRot._32 = vUp.z; matRot._33 = vFront.z;

		m_matView = matPos * matRot;
	}

	{ // projection matrix
		if (m_ProjType == PROJ_TYPE::ORTHOGRAPHICS)
		{
			Vec2 vResol = Device::GetInst()->GetResolution();
			m_matProj = DirectX::XMMatrixOrthographicLH(vResol.x * m_Scale, (vResol.x / m_AspectRatio) * m_Scale, 1.f, m_Far);
		}
		else if (m_ProjType == PROJ_TYPE::PERSPECTIVE)
		{
			m_matProj = DirectX::XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
		}
	}

	g_Transform.matView = m_matView;
	g_Transform.matProj = m_matProj;
}
