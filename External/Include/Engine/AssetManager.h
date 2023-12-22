#pragma once
#include "define.h"
#include "Asset.h"

class AssetManager 
{
	SINGLETON(AssetManager)
private:
	std::map<std::wstring, Asset*> m_Assets[(UINT)AssetType::END];
	//std::vector<std::wstring> m_AssetIDs[(UINT)AssetType::END];
	std::map<std::wstring, std::wstring> m_AssetPaths;

public:	// 방식??Id �?가지?�게 ?�니??wstring?�나 int ?�?�으�??�값??가지�?그걸 ?�일 ?�름?�로 ?�?�하�??�듯, ?�재 방식?� ?�용?��? ?�을 �?
	void Init();
	
	template <typename T>
	T* Load(const std::wstring& _strKey)
	{
		auto asset = m_Assets.find(_strKey);
		if (asset != m_Assets.end())
			return dynamic_cast<T*>(asset->second);

		auto iter = m_AssetPaths.find(_strKey);
		if (iter != m_AssetPaths.end())
		{
			std::wstring path = iter->second;

			Asset* asset = new T();
			if (asset->Load(path))
				return dynamic_cast<T*>(asset);
		}
		return nullptr;
	}

	void Save(Asset* _asset) { _asset->Save(); }

	template <typename T>
	bool Create(const std::wstring& _strKey, std::wstring _resourcePath, std::wstring _resourceName = L"resource")
	{
		Asset* asset = new T();
		asset->SetAssetID(_strKey);
		return asset->Create(_resourcePath, _resourceName);
	}
};

