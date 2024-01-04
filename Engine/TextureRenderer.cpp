#include "pch.h"
#include "TextureRenderer.h"
#include "Actor.h"

TextureRenderer::TextureRenderer() : Super(ComponentType::RENDERER)
	, m_Texture(nullptr)
	, m_Offset()
{
}

TextureRenderer::~TextureRenderer()
{
}

void TextureRenderer::Update()
{
}

void TextureRenderer::LateUpdate()
{
}

void TextureRenderer::Render()
{
	if (m_Texture != nullptr)
	{
		m_Texture->Render(GetOwner()->GetPosition() + m_Offset);
	}
}
