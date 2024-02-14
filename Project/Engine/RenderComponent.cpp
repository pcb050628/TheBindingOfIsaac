#include "pch.h"
#include "RenderComponent.h"

#include "Material.h"

RenderComponent::RenderComponent(COMPONENT_TYPE _type)
	: Component(_type)
{
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