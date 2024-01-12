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

	GetOwner()->GetTransform()->UpdateData();
}

void MeshRenderer::Render()
{
	if (nullptr == GetMesh() || nullptr == GetMaterial())
		return;

	if (GetOwner()->GetAnimator2D())
	{
		GetOwner()->GetAnimator2D()->UpdateData();
	}
	else
	{
		Animator2D::Clear();
	}

	UpdateData();

	GetMesh()->Render();
}