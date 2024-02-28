#include "pch.h"
#include "RenderComponent.h"

#include "Material.h"

#include "ResourceManager.h"

RenderComponent::RenderComponent(COMPONENT_TYPE _type)
	: Component(_type)
{
}

RenderComponent::RenderComponent(const RenderComponent& _origin)
	: Component(_origin)
{
	SetMesh(ResourceManager::GetInst()->Find<Mesh>(_origin.m_Mesh->GetResourceName()));

	SetMaterial(ResourceManager::GetInst()->Find<Material>(_origin.m_SharedMaterial->GetResourceName()));
	if (_origin.m_CurMaterial == m_DynamicMaterial)
	{
		GetDynamicMaterial();
		m_DynamicMaterial = _origin.m_DynamicMaterial->Clone();
	}
}

RenderComponent::~RenderComponent()
{
}

void RenderComponent::SetMaterial(Material* _material)
{
	if (m_DynamicMaterial)
		delete m_DynamicMaterial;

	m_CurMaterial = m_SharedMaterial = _material;
}

Material* RenderComponent::GetDynamicMaterial()
{
	if (m_DynamicMaterial)
		return m_DynamicMaterial;

	m_CurMaterial = m_DynamicMaterial = m_SharedMaterial->Clone();
	return m_DynamicMaterial;
}

void RenderComponent::SaveToFile(FILE* _file)
{
	SaveResourceRef(m_Mesh, _file);
	SaveResourceRef(m_SharedMaterial, _file);
}

void RenderComponent::LoadFromFile(FILE* _file)
{
	LoadResourceRef(m_Mesh, _file);

	Material* mtrl = nullptr;
	LoadResourceRef(mtrl, _file);
	SetMaterial(mtrl);
}
