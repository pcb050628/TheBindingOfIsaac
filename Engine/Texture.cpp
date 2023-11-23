#include "pch.h"
#include "Texture.h"
#include "RenderManager.h"
#include "directxtk/WICTextureLoader.h"

Texture::Texture() : Super(ResourceType::Texture)
	, m_TextureView(nullptr)
	, m_ImageSection({})
{
}

Texture::~Texture()
{
}

void Texture::Render()
{
	RenderManager::GetInst()->TextureRender(m_TextureView.Get(), m_ImageSection);
}

Texture* Texture::Create(std::wstring _ResourcePath)
{
	HRESULT hResult = DirectX::CreateWICTextureFromFile(static_cast<ID3D11Device*>(RenderManager::GetInst()->GetDevice().Get())
		, _ResourcePath.c_str(), nullptr, m_TextureView.GetAddressOf());
	if (SUCCEEDED(hResult))
		return this;
	else
		return nullptr;
}

bool Texture::Load(std::wstring _FilePath)
{
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _FilePath.c_str(), L"r");

	while (true)
	{
		wchar_t szRead[256] = {};
		if (EOF == fwscanf_s(pFile, L"%s", szRead, 256))
		{
			break;
		}

		if (!wcscmp(szRead, L"[TEXTURE_NAME]"))
		{
			fwscanf_s(pFile, L"%s", szRead, 256);
			Super::SetResourceName(szRead);
		}
		else if (!wcscmp(szRead, L"[TEXTURE_PATH]"))
		{
			std::wstring strRelativePath;

			fwscanf_s(pFile, L"%s", szRead, 256);
			strRelativePath = szRead;

			HRESULT hResult = DirectX::CreateWICTextureFromFile(static_cast<ID3D11Device*>(RenderManager::GetInst()->GetDevice().Get())
				, strRelativePath.c_str(), nullptr, m_TextureView.ReleaseAndGetAddressOf());
		}
		else if (!wcscmp(szRead, L"[LEFT]"))
		{
			int value = 0;
			fwscanf_s(pFile, L"%d", value, 256);
			m_ImageSection.left = value;
		}
		else if (!wcscmp(szRead, L"[TOP]"))
		{
			int value = 0;
			fwscanf_s(pFile, L"%d", value, 256);
			m_ImageSection.top = value;
		}
		else if (!wcscmp(szRead, L"[RIGHT]"))
		{
			int value = 0;
			fwscanf_s(pFile, L"%d", value, 256);
			m_ImageSection.right = value;
		}
		else if (!wcscmp(szRead, L"[BOTTOM]"))
		{
			int value = 0;
			fwscanf_s(pFile, L"%d", value, 256);
			m_ImageSection.bottom = value;

			break;
		}
	}

	return true;
}

bool Texture::Save()
{
	FILE* pFile = nullptr;

	std::wstring _FilePath = Super::GetResourcePath();

	_wfopen_s(&pFile, _FilePath.c_str(), L"w");

	/*if (nullptr == pFile)
	{
		LOG(ERR, L"파일 열기 실패");
		return false;
	}*/

	// 리소스 이름 저장
	fwprintf_s(pFile, L"[TEXTURE_NAME]\n");

	std::wstring strName = Super::GetResourceName();
	fwprintf_s(pFile, strName.c_str());
	fwprintf_s(pFile, L"\n\n");


	// 경로
	fwprintf_s(pFile, L"[TEXTURE_PATH]\n");

	fwprintf_s(pFile, Super::GetResourcePath().c_str());
	fwprintf_s(pFile, L"\n\n");

	// 경로
	fwprintf_s(pFile, L"[TEXTURE_SECTION]\n");

	fwprintf_s(pFile, L"[LEFT]\n");
	fwprintf_s(pFile, L"%d\n", m_ImageSection.left);

	fwprintf_s(pFile, L"[TOP]\n");
	fwprintf_s(pFile, L"%d\n", m_ImageSection.top);

	fwprintf_s(pFile, L"[RIGHT]\n");
	fwprintf_s(pFile, L"%d\n", m_ImageSection.right);

	fwprintf_s(pFile, L"[BOTTOM]\n");
	fwprintf_s(pFile, L"%d\n", m_ImageSection.bottom);

	fclose(pFile);

	return true;
}
