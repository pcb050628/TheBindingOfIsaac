#include "pch.h"
#include "MeshRenderer.h"

#include "GameObject.h"

#include "Transform.h"

#include "Mesh.h"
#include "GraphicsShader.h"

MeshRenderer::MeshRenderer() : RenderComponent(COMPONENT_TYPE::MESHRENDERER)
{
}

MeshRenderer::~MeshRenderer()
{
}


void MeshRenderer::UpdateData()
{
	if(GetShader() != nullptr)
		GetShader()->UpdateData();

	GetOwner()->GetComponent(COMPONENT_TYPE::TRANSFORM)->UpdateData();
}

void MeshRenderer::Render()
{
	if (GetMesh() == nullptr)
		return;

	UpdateData();
	GetMesh()->Render();
}