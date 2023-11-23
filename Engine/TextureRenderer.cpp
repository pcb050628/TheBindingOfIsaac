#include "pch.h"
#include "TextureRenderer.h"

TextureRenderer::TextureRenderer() : Super(ComponentType::RENDERER)
	, m_Texture(nullptr)
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
		m_Texture->Render();
	}
}
