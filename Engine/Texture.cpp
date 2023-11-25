#include "pch.h"
#include "Texture.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "math.h"

Texture::Texture() : Super(AssetType::TEXTURE)
	, m_TextureResource(nullptr)
	, m_ImageSection({})
{
}

Texture::~Texture()
{
}

void Texture::Render(Vec2 _pos)
{
	RenderManager::GetInst()->TextureRender(m_TextureResource->GetTextureView().Get(), _pos, m_ImageSection);
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
			return false;
		}
		
		if (!wcscmp(szRead, L"[TEXTURE_NAME]"))
		{
			std::wstring name;

			fwscanf_s(pFile, L"%s", szRead, 256);
			name = szRead;

			Super::SetResourceName(name);
		}
		else if (!wcscmp(szRead, L"[TEXTURE_PATH]"))
		{
			std::wstring path;

			fwscanf_s(pFile, L"%s", szRead, 256);
			path = szRead;

			m_TextureResource = ResourceManager::GetInst()->LoadByPath<ShaderTextureResource>(Super::GetResourceName(), path);
		}
		else if (!wcscmp(szRead, L"[LEFT]"))
		{
			int value = 0;
			fwscanf_s(pFile, L"%d", value);
			m_ImageSection.left = value;
		}
		else if (!wcscmp(szRead, L"[TOP]"))
		{
			int value = 0;
			fwscanf_s(pFile, L"%d", value);
			m_ImageSection.top = value;
		}
		else if (!wcscmp(szRead, L"[RIGHT]"))
		{
			int value = 0;
			fwscanf_s(pFile, L"%d", value);
			m_ImageSection.right = value;
		}
		else if (!wcscmp(szRead, L"[BOTTOM]"))
		{
			int value = 0;
			fwscanf_s(pFile, L"%d", value);
			m_ImageSection.bottom = value;

			break;
		}
	}

	return true;
}

bool Texture::Save()
{
	FILE* pFile = nullptr;

	std::wstring _FilePath = Super::GetResourcePath() + std::to_wstring((UINT)Super::GetAssetID());

	_wfopen_s(&pFile, _FilePath.c_str(), L"w");

	/*if (nullptr == pFile)
	{
		LOG(ERR, L"파일 열기 실패");
		return false;
	}*/

	// 이름
	fwprintf_s(pFile, L"[TEXTURE_NAME]\n");

	fwprintf_s(pFile, Super::GetResourceName().c_str());
	fwprintf_s(pFile, L"\n\n");

	// 경로
	fwprintf_s(pFile, L"[TEXTURE_PATH]\n");

	fwprintf_s(pFile, Super::GetResourcePath().c_str());
	fwprintf_s(pFile, L"\n\n");

	// image section
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
