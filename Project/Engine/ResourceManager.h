#pragma once
#include "pch.h"
#include "define.h"
#include "Resource.h"

#include "resources.h"

template <typename T>
RESOURCE_TYPE GetResourceType();

class ResourceManager
{
	SINGLETON(ResourceManager);
private:
	std::map<std::wstring, Resource*> m_Resources[(UINT)RESOURCE_TYPE::END];
	std::vector<std::wstring> m_ObjFile;
	
public:
	void Init();

private:
	void LoadAllContent(const std::wstring& _path);
	void CreateDefaultMesh();
	void CreateDefaultShader();
	void CreateDefaultMaterial();
	void LoadAnim();

	int LoadResource(const std::wstring& _path);

public:
	template <typename T>
	T* Find(std::wstring _strKey)
	{
		RESOURCE_TYPE type = GetResourceType<T>();

		if (m_Resources[(UINT)type].find(_strKey) != m_Resources[(UINT)type].end())
			return dynamic_cast<T*>(m_Resources[(UINT)type].find(_strKey)->second);
		return nullptr;
	}

	bool IsExist(const std::wstring& _strKey, RESOURCE_TYPE _type)
	{
		if (m_Resources[(UINT)_type].find(_strKey) != m_Resources[(UINT)_type].end())
			return true;
		else
			return false;
	}

	template <typename T>
	T* Load(std::wstring _FileName, bool _isFullPath = false)
	{
		Resource* tmp = new T();
		if (!tmp->Load(_FileName, _isFullPath))
		{
			delete tmp;
			return nullptr;
		}
		else
		{
			if (IsExist(tmp->GetResourceName(), tmp->GetResourceType()))
			{
				delete tmp;

				// 이미 같은 이름의 리소스가 존재할땐 이미 존재하던 리소스를 반환함
				wchar_t szName[20] = {};
				_wsplitpath_s(_FileName.c_str(), nullptr, 0, nullptr, 0, szName, 20, nullptr, 0);

				return Find<T>(szName);
			}

			m_Resources[(UINT)tmp->GetResourceType()].insert(std::make_pair(tmp->GetResourceName(), tmp));
			return dynamic_cast<T*>(tmp);
		}

		return nullptr;
	}

	int AddResource(std::wstring _strKey, Resource* _resource, bool _bForceInput = false)
	{
		if (IsExist(_strKey, _resource->GetResourceType()) && !_bForceInput)
		{
			MessageBoxW(nullptr, L"이미 존재하는 키 값 입니다", L"리소스 추가 실패", MB_OK);
			return E_FAIL;
		}

		m_Resources[(UINT)_resource->GetResourceType()].insert(std::make_pair(_strKey, _resource));
		return S_OK;
	}

	int AddResource(Resource* _resource, bool _bForceInput = false)
	{
		if (IsExist(_resource->GetResourceName(), _resource->GetResourceType()) && !_bForceInput)
		{
			MessageBoxW(nullptr, L"이미 존재하는 키 값 입니다", L"리소스 추가 실패", MB_OK);
			return E_FAIL;
		}

		m_Resources[(UINT)_resource->GetResourceType()].insert(std::make_pair(_resource->GetResourceName(), _resource));
		return S_OK;
	}

	Texture* CreateTexture(const std::wstring& _strKey
		, UINT _width, UINT _height, DXGI_FORMAT _format, UINT _bindFlags, D3D11_USAGE _usage = D3D11_USAGE_DEFAULT);

	Texture* CreateTexture(const std::wstring& _strKey, Microsoft::WRL::ComPtr<ID3D11Texture2D> _tex2D);

	void GetAssetName(RESOURCE_TYPE _type, std::vector<std::string>& _vecStr);


};

template <typename T>
RESOURCE_TYPE GetResourceType()
{
	const type_info& type = typeid(T);

	RESOURCE_TYPE value = RESOURCE_TYPE::END;

	if (&type == &typeid(Mesh))
		return RESOURCE_TYPE::MESH;
	else if (&type == &typeid(GraphicsShader))
		return RESOURCE_TYPE::GRAPHICS_SHADER;
	else if (&type == &typeid(Material))
		return RESOURCE_TYPE::MATERIAL;
	else if (&type == &typeid(Texture))
		return RESOURCE_TYPE::TEXTURE;
	else if (&type == &typeid(Anim))
		return RESOURCE_TYPE::ANIM;

	return value;
}