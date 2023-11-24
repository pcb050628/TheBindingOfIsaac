#include "pch.h"
#include "ShaderTextureResource.h"
#include "RenderManager.h"
#include "directxtk/WICTextureLoader.h"

ShaderTextureResource::ShaderTextureResource() : Super(ResourceType::Texture)
{
}

ShaderTextureResource::~ShaderTextureResource()
{
}

bool ShaderTextureResource::Load(std::wstring _path)
{
	HRESULT hResult = DirectX::CreateWICTextureFromFile(static_cast<ID3D11Device*>(RenderManager::GetInst()->GetDevice().Get())
		, _path.c_str(), nullptr, m_TextureView.GetAddressOf());
	if (SUCCEEDED(hResult))
		return true;
	else
		return false;
}
