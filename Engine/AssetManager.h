#pragma once
#include "define.h"
#include "Asset.h"

class AssetManager
{
	SINGLETON(AssetManager)
private:
	std::map<UINT, Asset*> m_Assets;
	std::map<UINT, std::wstring> m_AssetPaths;

public:
	void Init();
	
	template <typename T>
	T* Load(UINT _id)
	{
		Asset* asset = new T();
		if (asset->Load())
			return static_cast<T*>(asset);
		return nullptr;
	}

	void Save(Asset* _asset) { _asset->Save(); }
};

