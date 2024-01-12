#include "pch.h"
#include "Transform.h"

#include "Device.h"
#include "ConstantBuffer.h"

#include "GameObject.h"

Transform::Transform() : Component(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativePos(Vec3(0, 0, 0))
	, m_vRelativeScale(Vec3(100, 100, 0))
	, m_vRelativeRot(Vec3(0, 0, 0))
	, m_bAbsolute(true)
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

	Matrix matTranslation = DirectX::XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

	Matrix matScale = DirectX::XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);

	Matrix matRotateX = DirectX::XMMatrixRotationX(m_vRelativeRot.x);
	Matrix matRotateY = DirectX::XMMatrixRotationY(m_vRelativeRot.y);
	Matrix matRotateZ = DirectX::XMMatrixRotationZ(m_vRelativeRot.z);

	m_matWorld = matScale * matRotateX * matRotateY * matRotateZ * matTranslation;

	m_vLocalDir[(UINT)DIR_TYPE::RIGHT] = Vec3(1.f, 0.f, 0.f);
	m_vLocalDir[(UINT)DIR_TYPE::UP]	  = Vec3(0.f, 1.f, 0.f);
	m_vLocalDir[(UINT)DIR_TYPE::FRONT] = Vec3(0.f, 0.f, 1.f);

	for (int i = 0; i < (UINT)DIR_TYPE::END; i++)
	{
		m_vLocalDir[i] = XMVector3TransformNormal(m_vLocalDir[i], m_matWorld);
		m_vWorldDir[i] = m_vLocalDir[i].Normalize();
	}

	if (GetOwner()->GetParent())
	{
		const Matrix& parentWorldMat = GetOwner()->GetParent()->GetTransform()->GetWorldMat();

		if (m_bAbsolute)
		{
			Vec3 parentScale = GetOwner()->GetParent()->GetTransform()->GetRelativeScale();
			Matrix parentScaleMatInv = DirectX::XMMatrixScaling(1.f / parentScale.x, 1.f / parentScale.y, 1.f / parentScale.z);

			m_matWorld = m_matWorld * parentScaleMatInv * parentWorldMat;
		}
		else
		{
			m_matWorld *= parentWorldMat;
		}

		for (int i = 0; i < (UINT)DIR_TYPE::END; i++)
		{
			m_vWorldDir[i] = DirectX::XMVector3TransformNormal(m_vWorldDir[i], m_matWorld);
			m_vWorldDir[i].Normalize();
		}
	}
}

void Transform::UpdateData()
{
	g_Transform.matWorld = m_matWorld;
	g_Transform.matWV = g_Transform.matWorld * g_Transform.matView;
	g_Transform.matWVP = g_Transform.matWV * g_Transform.matProj;

	ConstantBuffer* constBuffer = Device::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
	constBuffer->SetData(&g_Transform);
	constBuffer->UpdateData();
}

Vec3 Transform::GetWorldScale()
{
	GameObject* pParent = GetOwner()->GetParent();
	Vec3 vWorldScale = m_vRelativeScale;

	while (pParent)
	{
		vWorldScale *= pParent->GetTransform()->GetRelativeScale();

		pParent = pParent->GetParent();
	}

	return vWorldScale;
}
