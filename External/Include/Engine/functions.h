#pragma once

class Entity;
class GameObject;
enum class CHAPTER_LEVEL;
enum DIRECTION;
enum class LAYER_TYPE;
enum class RESOURCE_TYPE;
class Component;
enum class COMPONENT_TYPE;

bool IsValid(Entity* _entity);
void ChangeChapter(CHAPTER_LEVEL _level);
void AddGameObject(GameObject* _actr, LAYER_TYPE _layer);
void ChangeRoom(DIRECTION _dir);
void Destroy(GameObject* _obj);
std::wstring GetContentPath();
std::wstring GetResourceFolderPath(RESOURCE_TYPE _type);

void DrawDebugRect(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
void DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

void DrawDebugCross(Vec3 _vWorldPos, float _fScale, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

std::wstring ToWstring(const std::string& _str);
std::string ToString(const std::wstring& _str);

Component* GetComponentByComponentType(COMPONENT_TYPE _type);

#include "ResourceManager.h"
#include "Resource.h"

template <typename T>
void WriteData(void* data, std::wofstream& _file)
{
	_file.write(reinterpret_cast<const wchar_t*>(data), sizeof(T));
}

template <typename T>
void ReadData(void* data, std::wifstream& _file)
{
	_file.read(reinterpret_cast<wchar_t*>(data), sizeof(T));
}

//save resource's existance, name
void SaveResourceRef(Resource* _resource, std::wofstream& _file);

template<typename T>
int LoadResourceRef(Resource* _resouce, std::wifstream& _file)
{
	bool bAssetExist = false;
	_file.read(reinterpret_cast<wchar_t*>(bAssetExist), sizeof(bool));

	if (bAssetExist)
	{
		int len = 0;
		wstring name;
		_file.read(reinterpret_cast<wchar_t*>(len), sizeof(int));
		_file.read(name.data(), len);

		if (L"NULL" == name)
			return E_FAIL;

		_resouce = ResourceManager::GetInst()->Load<T>(name);
		return S_OK;
	}
}
