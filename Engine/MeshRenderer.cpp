#include "pch.h"
#include "MeshRenderer.h"

#include "GameObject.h"

#include "Transform.h"
#include "Animator2D.h"

#include "Mesh.h"
#include "Material.h"

MeshRenderer::MeshRenderer() : RenderComponent(COMPONENT_TYPE::MESHRENDERER)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::UpdateData()
{
	if(GetMaterial() != nullptr)
		GetMaterial()->UpdateData();

	GetOwner()->GetComponent<Transform>()->UpdateData();
}

void MeshRenderer::Render()
{
	if (GetMesh() == nullptr)
		return;

	UpdateData();
	GetMesh()->Render();
}