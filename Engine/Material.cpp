#include "pch.h"
#include "Material.h"

#include "Device.h"
#include "ResourceManager.h"

#include "ConstantBuffer.h"
#include "GraphicsShader.h"
#include "Texture.h"

Material::Material()
	: Resource(RESOURCE_TYPE::MATERIAL)
	, m_ConstData()
	, m_Textures{}
	, m_Shader()
{
}

Material::~Material()
{
}

bool Material::Load(const std::wstring& _strFilePath)
{
	filesystem::path filePath = _strFilePath;
	std::wifstream fileStream(filePath);

	wchar_t szName[20] = {};
	_wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, szName, 20, nullptr, 0);

	m_ResourceName = szName;
	m_ResourcePath = _strFilePath;

	if (fileStream.is_open())
	{
		std::wstring line;

		while (line != L"END")
		{
			std::getline(fileStream, line);

			if (line == L"[Shader_Name]")
			{
				std::getline(fileStream, line);
				m_Shader = ResourceManager::GetInst()->Find<GraphicsShader>(line);
			}
			else if (line == L"[Texture0_Name]")
			{
				std::getline(fileStream, line);
				if (line != L"NULL")
				{
					SetTexture(ResourceManager::GetInst()->Find<Texture>(line), TEX_PARAM::TEX_0);
				}
			}
			else if (line == L"[Texture1_Name]")
			{
				std::getline(fileStream, line);
				if (line != L"NULL")
				{
					SetTexture(ResourceManager::GetInst()->Find<Texture>(line), TEX_PARAM::TEX_0);
				}
			}
			else if (line == L"[Texture2_Name]")
			{
				std::getline(fileStream, line);
				if (line != L"NULL")
				{
					SetTexture(ResourceManager::GetInst()->Find<Texture>(line), TEX_PARAM::TEX_0);
				}
			}
			else if (line == L"[Texture3_Name]")
			{
				std::getline(fileStream, line);
				if (line != L"NULL")
				{
					SetTexture(ResourceManager::GetInst()->Find<Texture>(line), TEX_PARAM::TEX_0);
				}
			}
			else if (line == L"[Texture4_Name]")
			{
				std::getline(fileStream, line);
				if (line != L"NULL")
				{
					SetTexture(ResourceManager::GetInst()->Find<Texture>(line), TEX_PARAM::TEX_0);
				}
			}
			else if (line == L"[Texture5_Name]")
			{
				std::getline(fileStream, line);
				if (line != L"NULL")
				{
					SetTexture(ResourceManager::GetInst()->Find<Texture>(line), TEX_PARAM::TEX_0);
				}
			}


			else if (line == L"[Texture6_Name]")
			{
				std::getline(fileStream, line);
				if (line != L"NULL")
				{
					SetTexture(ResourceManager::GetInst()->Find<Texture>(line), TEX_PARAM::TEX_0);
				}
			}
			else if (line == L"[Texture7_Name]")
			{
				std::getline(fileStream, line);
				if (line != L"NULL")
				{
					SetTexture(ResourceManager::GetInst()->Find<Texture>(line), TEX_PARAM::TEX_0);
				}
			}


			else if (line == L"[Texture8_Name]")
			{
				std::getline(fileStream, line);
				if (line != L"NULL")
				{
					SetTexture(ResourceManager::GetInst()->Find<Texture>(line), TEX_PARAM::TEX_0);
				}
			}
			else if (line == L"[Texture9_Name]")
			{
				std::getline(fileStream, line);
				if (line != L"NULL")
				{
					SetTexture(ResourceManager::GetInst()->Find<Texture>(line), TEX_PARAM::TEX_0);
				}
			}
		}

		return true;
	}
	else
		return false;
}

bool Material::Save()
{
	filesystem::path filePath = GetContentPath() + L"Resource\\Material\\" + m_ResourceName;
	filePath += L".txt";
	std::wofstream fileStream(filePath);

	if (fileStream.is_open())
	{
		if(m_Shader != nullptr)
			fileStream << L"[Shader_Name]\n" << m_Shader->GetResourceName().c_str() << std::endl;
		else
			fileStream << L"[Shader_Name]\n" << L"NULL" << std::endl;
		
		for (int i = 0; i < TEX_PARAM::END; i++)
		{
			if (m_Textures[i] != nullptr)
				fileStream << L"[Texture" << i << "_Name]\n" << m_Textures[i]->GetResourceName() << std::endl;
			else
				fileStream << L"[Texture" << i << "_Name]\n" << "NULL" << std::endl;
		}

		fileStream << L"END";

		fileStream.close();

		return true;
	}
	else
		return false;

	return false;
}

void Material::UpdateData()
{
	if (m_Shader != nullptr)
		m_Shader->UpdateData();

	ConstantBuffer* cBuffer = Device::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL_CONST);
	cBuffer->SetData(&m_ConstData);
	cBuffer->UpdateData();
}
