#include "pch.h"
#include "Prefab.h"

#include "GameObject.h"

SAVE_TYPE Prefab::m_SaveFunc = nullptr;
LOAD_TYPE Prefab::m_LoadFunc = nullptr;

Prefab::Prefab() : Resource(RESOURCE_TYPE::PREFAB)
    ,m_ProtoObj(nullptr)
{
}

Prefab::~Prefab()
{
}

bool Prefab::Load(const std::wstring& _FileName, bool _isFullPath)
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

	SaveWString(m_ResourceName, pFile);

	m_ProtoObj = m_LoadFunc(pFile);

	fclose(pFile);

    return true;
}

bool Prefab::Save()
{
	filesystem::path filePath = GetContentPath() + GetResourceFolderPath(m_Type) + m_ResourceName;
	filePath += L".pref";

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, filePath.c_str(), L"wb");

	if (nullptr == pFile)
		return false;

	m_SaveFunc(m_ProtoObj, pFile);

	fclose(pFile);

    return true;
}
