#include "pch.h"
#include "Light2D.h"

#include "RenderManager.h"

#include "GameObject.h"
#include "Transform.h"

Light2D::Light2D() : Component(COMPONENT_TYPE::LIGHT2D)
{
}

Light2D::~Light2D()
{
}

void Light2D::LateUpdate()
{
	Vec3 vPos = GetOwner()->GetTransform()->GetWorldPos();
	Vec3 vDir = GetOwner()->GetTransform()->GetWorldDir(DIR_TYPE::RIGHT);

	m_Info.vWorldPos = vPos;
	m_Info.vWorldDir = vDir;

	RenderManager::GetInst()->RegisterLight2D(this);
}

void Light2D::SaveToFile(FILE* _file)
{
	fwrite(&m_Info, sizeof(tLightInfo), 1, _file);
}

void Light2D::LoadFromFile(FILE* _file)
{
	fread(&m_Info, sizeof(tLightInfo), 1, _file);
}
