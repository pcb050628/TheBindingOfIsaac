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

void Material::UpdateData()
{
	if (nullptr == m_Shader)
		return;

	// 사용할 쉐이더 바인딩
	m_Shader->UpdateData();

	// Texture Update(Register Binding)
	for (UINT i = 0; i < TEX_PARAM::END; ++i)
	{
		if (nullptr != m_Textures[i])
		{
			m_Textures[i]->UpdateData(i);
			m_ConstData.bTex[i] = 1;
		}
		else
		{
			Texture::Clear(i);
			m_ConstData.bTex[i] = 0;
		}
	}

	// 상수 데이터 업데이트
	static ConstantBuffer* pCB = Device::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL_CONST);
	pCB->SetData(&m_ConstData);
	pCB->UpdateData();
}

bool Material::Load(const std::wstring& _FileName, bool _isFullPath)
{
	filesystem::path filePath;

	if (_isFullPath)
		filePath = _FileName;
	else
		filePath = GetContentPath() + GetResourceFolderPath(m_Type) + _FileName;

	wchar_t szName[20] = {};
	_wsplitpath_s(filePath.c_str(), nullptr, 0, nullptr, 0, szName, 20, nullptr, 0);

	m_ResourceName = szName;
	m_ResourcePath = _FileName;

	FILE* pFile = nullptr; 
	_wfopen_s(&pFile, filePath.c_str(), L"rb"); 

	if (nullptr == pFile) 
		return false; 

	// 재질 상수값 저장
	fread(&m_ConstData, sizeof(tMtrlData), 1, pFile); 

	// 재질이 참조하는 텍스쳐 정보를 로드
	for (UINT i = 0; i < (UINT)TEX_PARAM::END; ++i)
	{
		LOADRESOURCEREF(Texture, m_Textures[i], pFile)
	}

	// 재질이 참조하는 쉐이더 정보를 저장
	LOADRESOURCEREF(GraphicsShader, m_Shader, pFile)

	return true;

	//std::wifstream fileStream(filePath);
	//
	//if (fileStream.is_open())
	//{
	//	std::wstring line;
	//
	//	while (!fileStream.eof())
	//	{
	//		std::getline(fileStream, line);
	//
	//		if (line == L"[Shader_Name]")
	//		{
	//			std::getline(fileStream, line);
	//			m_Shader = ResourceManager::GetInst()->Find<GraphicsShader>(line);
	//		}
	//		else if (line == L"[Texture]")
	//		{
	//			std::getline(fileStream, line);
	//			int idx = stoi(line.c_str());
	//			std::getline(fileStream, line);
	//			if (line != L"NULL")
	//			{
	//				SetTexture(ResourceManager::GetInst()->Find<Texture>(line), TEX_PARAM(idx));
	//			}
	//		}
	//	}
	//
	//	fileStream.close();
	//	return true;
	//}
	//else
	//	return false;
}

bool Material::Save()
{
	filesystem::path filePath = GetContentPath() + GetResourceFolderPath(m_Type) + m_ResourceName;
	filePath += L".mtrl";

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, filePath.c_str(), L"wb");

	if (nullptr == pFile)
		return false;

	// 재질 상수값 저장
	fwrite(&m_ConstData, sizeof(tMtrlData), 1, pFile);

	//texture
	for (int i = 0; i < TEX_PARAM::END; i++)
	{
		//SaveResourceRef(m_Textures[i], pFile);
		SAVERESOURCEREF(m_Textures[i], pFile);
	}

	//shader
	SAVERESOURCEREF(m_Shader, pFile);

	return true;

	//std::wofstream fileStream(filePath);
	//
	//if (fileStream.is_open())
	//{
	//	if (m_Shader != nullptr)
	//		fileStream << L"[Shader_Name]\n" << m_Shader->GetResourceName().c_str() << std::endl;
	//	else
	//		fileStream << L"[Shader_Name]\n" << L"NULL" << std::endl;
	//
	//	for (int i = 0; i < TEX_PARAM::END; i++)
	//	{
	//		if (m_Textures[i] != nullptr)
	//			fileStream << L"[Texture]\n" << i << std::endl << m_Textures[i]->GetResourceName() << std::endl;
	//		else
	//			fileStream << L"[Texture]\n" << i << std::endl << "NULL" << std::endl;
	//	}
	//
	//	fileStream.close();
	//
	//	return true;
	//}
	//else
	//	return false;
	//
	//return false;
}