#include "pch.h"
#include "Texture.h"

#include "Device.h"

Texture::Texture() : Resource(RESOURCE_TYPE::TEXTURE)
	, m_Image()
	, m_Tex2D()
	, m_Desc{}
	, m_RSView()
	, m_DSView()
	, m_SRView()
	, m_UAView()
{
}

Texture::~Texture()
{
}

bool Texture::Load(const std::wstring& _strFilePath)
{
	wchar_t szExt[20] = {};
	_wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 20);

	HRESULT hr = S_OK;

	if (!wcscmp(szExt, L".dds") || !wcscmp(szExt, L".DDS"))
	{
		hr = LoadFromDDSFile(_strFilePath.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, m_Image);
	}

	else if (!wcscmp(szExt, L".tga") || !wcscmp(szExt, L".TGA"))
	{
		hr = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
	}

	else
	{
		// png, bmp, jpg, jpeg
		hr = LoadFromWICFile(_strFilePath.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, m_Image);
	}

	if (FAILED(hr))
		return false;

	DirectX::CreateShaderResourceView(
		Device::GetInst()->GetDevice().Get(), m_Image.GetImages(), m_Image.GetImageCount(), m_Image.GetMetadata(), m_SRView.GetAddressOf());
	m_SRView->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());
	m_Tex2D->GetDesc(&m_Desc);

    return true;
}

void Texture::UpdateData(int _regiNum)
{
	Device::GetInst()->GetContext()->VSSetShaderResources(_regiNum, 1, m_SRView.GetAddressOf());
	Device::GetInst()->GetContext()->HSSetShaderResources(_regiNum, 1, m_SRView.GetAddressOf());
	Device::GetInst()->GetContext()->DSSetShaderResources(_regiNum, 1, m_SRView.GetAddressOf());
	Device::GetInst()->GetContext()->GSSetShaderResources(_regiNum, 1, m_SRView.GetAddressOf());
	Device::GetInst()->GetContext()->PSSetShaderResources(_regiNum, 1, m_SRView.GetAddressOf());
}
