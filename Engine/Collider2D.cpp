#include "pch.h"
#include "Collider2D.h"

#include "GameObject.h"
#include "Transform.h"

Collider2D::Collider2D() : Component(COMPONENT_TYPE::COLLIDER2D)
	, m_bAbsolute(false)
	, m_Type(COLLIDER2D_TYPE::BOX)
	, m_vOffsetPos(0.f)
	, m_vOffsetScale(1.f)
	, m_matColWorld{}
{
}

Collider2D::~Collider2D()
{
}

void Collider2D::LateUpdate()
{
	m_matColWorld
		= DirectX::XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z) * DirectX::XMMatrixTranslation(m_vOffsetPos.x, m_vOffsetPos.y, m_vOffsetPos.z);

	const Matrix& matObjWorld = GetOwner()->GetTransform()->GetWorldMat();

	if (m_bAbsolute)
	{
		Vec3 vObjScaleInv = GetOwner()->GetTransform()->GetWorldScale();
		Matrix matObjScaleInv = DirectX::XMMatrixScaling(vObjScaleInv.x, vObjScaleInv.y, vObjScaleInv.z);
		matObjScaleInv = DirectX::XMMatrixInverse(nullptr, matObjScaleInv);

		m_matColWorld = m_matColWorld * matObjScaleInv * matObjWorld;
	}
	else
	{
		m_matColWorld *= matObjWorld;
	}
}
