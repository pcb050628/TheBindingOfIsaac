#include "pch.h"
#include "Texture.h"

#include "Device.h"

Texture::Texture() : Resource(RESOURCE_TYPE::TEXTURE)
	, m_Image()
	, m_Tex2D()
	, m_Desc{}
	, m_RTView()
	, m_DSView()
	, m_SRView()
	, m_UAView()
{
}

Texture::~Texture()
{
}

bool Texture::Load(const std::wstring& _FileName)
{
	std::wstring fullPath = GetContentPath() + GetResourceFolderPath(m_Type) + _FileName;

	wchar_t szExt[20] = {};
	_wsplitpath_s(_FileName.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 20);

	wchar_t szName[20] = {};
	_wsplitpath_s(_FileName.c_str(), nullptr, 0, nullptr, 0, szName, 20, nullptr, 0);

	m_ResourceName = szName;

	HRESULT hr = S_OK;

	if (!wcscmp(szExt, L".dds") || !wcscmp(szExt, L".DDS"))
	{
		hr = LoadFromDDSFile(fullPath.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, m_Image);
	}

	else if (!wcscmp(szExt, L".tga") || !wcscmp(szExt, L".TGA"))
	{
		hr = LoadFromTGAFile(fullPath.c_str(), nullptr, m_Image);
	}

	else
	{
		// png, bmp, jpg, jpeg
		hr = LoadFromWICFile(fullPath.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, m_Image);
	}

	if (FAILED(hr))
		return false;

	DirectX::CreateShaderResourceView(
		Device::GetInst()->GetDevice().Get(), m_Image.GetImages(), m_Image.GetImageCount(), m_Image.GetMetadata(), m_SRView.GetAddressOf());
	m_SRView->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());
	m_Tex2D->GetDesc(&m_Desc);

	m_ResourcePath = _FileName;

    return true;
}

int Texture::Create(UINT _Width, UINT _Height, DXGI_FORMAT _Format, UINT _BindFlag, D3D11_USAGE _Usage)
{
	HRESULT hResult = S_OK;

	m_Desc.Format = _Format;

	m_Desc.BindFlags = _BindFlag;

	m_Desc.Width = _Width;
	m_Desc.Height = _Height;

	m_Desc.SampleDesc.Count = 1;
	m_Desc.SampleDesc.Quality = 0;

	m_Desc.MipLevels = 1;
	m_Desc.MiscFlags = 0;

	m_Desc.ArraySize = 1;

	m_Desc.Usage = _Usage;
	if (_Usage == D3D11_USAGE_DYNAMIC)
	{
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	hResult = Device::GetInst()->GetDevice()->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf());
	if (FAILED(hResult)) return E_FAIL;

	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		hResult = Device::GetInst()->GetDevice()->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSView.GetAddressOf());
		if (FAILED(hResult)) return E_FAIL;
	}
	else
	{
		if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			hResult = Device::GetInst()->GetDevice()->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRView.GetAddressOf());
			if (FAILED(hResult)) return E_FAIL;
		}

		if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
		{
			hResult = Device::GetInst()->GetDevice()->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTView.GetAddressOf());
			if (FAILED(hResult)) return E_FAIL;
		}

		if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			hResult = Device::GetInst()->GetDevice()->CreateUnorderedAccessView(m_Tex2D.Get(), nullptr, m_UAView.GetAddressOf());
			if (FAILED(hResult)) return E_FAIL;
		}
	}

	return S_OK;
}

int Texture::Create(Microsoft::WRL::ComPtr<ID3D11Texture2D> _tex2D)
{
	assert(_tex2D.Get());

	m_Tex2D = _tex2D;
	m_Tex2D->GetDesc(&m_Desc);

	HRESULT hResult = S_OK;

	if (FAILED(hResult)) return E_FAIL;

	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		hResult = Device::GetInst()->GetDevice()->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSView.GetAddressOf());
		if (FAILED(hResult)) return E_FAIL;
	}
	else
	{
		if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			hResult = Device::GetInst()->GetDevice()->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRView.GetAddressOf());
			if (FAILED(hResult)) return E_FAIL;
		}

		if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
		{
			hResult = Device::GetInst()->GetDevice()->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTView.GetAddressOf());
			if (FAILED(hResult)) return E_FAIL;
		}

		if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
		{
			hResult = Device::GetInst()->GetDevice()->CreateUnorderedAccessView(m_Tex2D.Get(), nullptr, m_UAView.GetAddressOf());
			if (FAILED(hResult)) return E_FAIL;
		}
	}

	return S_OK;
}

void Texture::UpdateData(int _regiNum)
{
	Device::GetInst()->GetContext()->VSSetShaderResources(_regiNum, 1, m_SRView.GetAddressOf());
	Device::GetInst()->GetContext()->HSSetShaderResources(_regiNum, 1, m_SRView.GetAddressOf());
	Device::GetInst()->GetContext()->DSSetShaderResources(_regiNum, 1, m_SRView.GetAddressOf());
	Device::GetInst()->GetContext()->GSSetShaderResources(_regiNum, 1, m_SRView.GetAddressOf());
	Device::GetInst()->GetContext()->PSSetShaderResources(_regiNum, 1, m_SRView.GetAddressOf());
}

void Texture::Clear(int _regiNum)
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	Device::GetInst()->GetContext()->VSSetShaderResources(_regiNum, 1, &pSRV);
	Device::GetInst()->GetContext()->HSSetShaderResources(_regiNum, 1, &pSRV);
	Device::GetInst()->GetContext()->DSSetShaderResources(_regiNum, 1, &pSRV);
	Device::GetInst()->GetContext()->GSSetShaderResources(_regiNum, 1, &pSRV);
	Device::GetInst()->GetContext()->PSSetShaderResources(_regiNum, 1, &pSRV);
}
