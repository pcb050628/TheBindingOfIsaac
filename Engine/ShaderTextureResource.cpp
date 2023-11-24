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
	HRESULT hResult = DirectX::CreateWICTextureFromFile(dynamic_cast<ID3D11Device*>(RenderManager::GetInst()->GetDevice().Get())
		, _path.c_str(), nullptr, m_TextureView.GetAddressOf());

	//std::wstring string = _path;
	//std::wstring name = string.substr(string.rfind(L"\\"), string.size());
	//SetName(name);
	if (SUCCEEDED(hResult))
		return true;
	else
		return false;
}
