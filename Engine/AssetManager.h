#pragma once
#include "define.h"
#include "Asset.h"

class AssetManager
{
	SINGLETON(AssetManager)
private:
	std::map<AssetID, Asset*> m_Assets;
	std::vector<AssetID> m_AssetIDs[(UINT)AssetType::END];
	std::map<AssetID, std::wstring> m_AssetPaths;

public:
	void Init();
	
	template <typename T>
	T* Load(AssetID _id)
	{
		auto asset = m_Assets.find(_id);
		if (asset != m_Assets.end())
			return dynamic_cast<T*>(asset->second);

		auto iter = m_AssetPaths.find(_id);
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
	bool Create(AssetID _id, std::wstring _resourcePath, std::wstring _resourceName = L"resource")
	{
		Asset* asset = new T();
		asset->SetAssetID(_id);
		return asset->Create(_resourcePath, _resourceName);
	}
};

