#pragma once
#include "define.h"
#include "Asset.h"

class AssetManager
{
	SINGLETON(AssetManager)
private:
	std::map<AssetID, Asset*> m_Assets;
	std::map<AssetID, std::wstring> m_AssetPaths;

	//AssetType m_TypeArray[(UINT)AssetType::END];

public:
	void Init();
	
	template <typename T>
	T* Load(AssetID _id)
	{
		auto asset = m_Assets.find(_id);
		if (asset != m_Assets.end())
			return asset;

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
};

