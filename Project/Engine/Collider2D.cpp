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


void Collider2D::BeginOverlap(Collider2D* _other)
{
	m_iCollisionCount++;
	GetOwner()->BeginOverlap(_other);
}

void Collider2D::Overlap(Collider2D* _other)
{
	GetOwner()->Overlap(_other);
}

void Collider2D::EndOverlap(Collider2D* _other)
{
	m_iCollisionCount--;
	GetOwner()->EndOverlap(_other);
}

void Collider2D::SaveToFile(FILE* _file)
{
	fwrite(&m_vOffsetPos, sizeof(Vec3), 1, _file);
	fwrite(&m_vOffsetScale, sizeof(Vec3), 1, _file);
	fwrite(&m_bAbsolute, sizeof(bool), 1, _file);
}

void Collider2D::LoadFromFile(FILE* _file)
{
	fread(&m_vOffsetPos, sizeof(Vec3), 1, _file);
	fread(&m_vOffsetScale, sizeof(Vec3), 1, _file);
	fread(&m_bAbsolute, sizeof(bool), 1, _file);
}