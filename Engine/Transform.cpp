#include "pch.h"
#include "Transform.h"

#include "Device.h"
#include "ConstantBuffer.h"

Transform::Transform() : Component(COMPONENT_TYPE::TRANSFORM)
{
}

Transform::~Transform()
{
}

void Transform::Update()
{
}

void Transform::LateUpdate()
{
	m_matWorld = DirectX::XMMatrixIdentity();

	Matrix matPosition = DirectX::XMMatrixTranslation(m_vPosition.x, m_vPosition.y, m_vPosition.z);

	Matrix matScale = DirectX::XMMatrixScaling(m_vScale.x, m_vScale.y, m_vScale.z);

	Matrix matRotateX = DirectX::XMMatrixRotationX(m_vRotation.x);
	Matrix matRotateY = DirectX::XMMatrixRotationX(m_vRotation.y);
	Matrix matRotateZ = DirectX::XMMatrixRotationX(m_vRotation.z);

	m_matWorld = matScale * matRotateX * matRotateY * matRotateZ * matPosition;

	m_vDir[(UINT)DIR_TYPE::RIGHT] = Vec3(1.f, 0.f, 0.f);
	m_vDir[(UINT)DIR_TYPE::UP]	  = Vec3(0.f, 1.f, 0.f);
	m_vDir[(UINT)DIR_TYPE::FRONT] = Vec3(0.f, 0.f, 1.f);

	for (int i = 0; i < (UINT)DIR_TYPE::END; i++)
	{
		m_vDir[i] = DirectX::XMVector3TransformNormal(m_vDir[i], m_matWorld);
		m_vDir[i].Normalize();
	}
}

void Transform::UpdateData()
{
	g_Transform.matWorld = m_matWorld;

	ConstantBuffer* constBuffer = Device::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	constBuffer->SetData(&g_Transform);
	constBuffer->UpdateData();
}
