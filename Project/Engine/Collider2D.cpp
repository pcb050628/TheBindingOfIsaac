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
	, m_bDebugDraw(false)
{
}

Collider2D::~Collider2D()
{
}

void Collider2D::LateUpdate()
{
	m_matColWorld = DirectX::XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z);
	m_matColWorld *= DirectX::XMMatrixTranslation(m_vOffsetPos.x, m_vOffsetPos.y, m_vOffsetPos.z);

	const Matrix& matObjWorld = GetOwner()->GetTransform()->GetWorldMat();

	if (m_bAbsolute)
	{
		Vec3 vObjScale = GetOwner()->GetTransform()->GetWorldScale();
		Matrix matObjScaleInv = DirectX::XMMatrixScaling(vObjScale.x, vObjScale.y, vObjScale.z);
		matObjScaleInv = DirectX::XMMatrixInverse(nullptr, matObjScaleInv);

		m_matColWorld = m_matColWorld * matObjScaleInv * matObjWorld;
	}
	else
	{
		m_matColWorld *= matObjWorld;
	}

	if (m_bDebugDraw)
	{
		DrawDebugRect(m_matColWorld, Vec3(0, 1, 0), false);
	}
}
