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
std::wstring GetResourceExt(RESOURCE_TYPE _type);

void DrawDebugRect(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
void DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

void DrawDebugCross(Vec3 _vWorldPos, float _fScale, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

std::wstring ToWstring(const std::string& _str);
std::string ToString(const std::wstring& _str);

Component* GetComponentByComponentType(COMPONENT_TYPE _type);

#include "ResourceManager.h"

template <typename T>
void SaveResourceRef(T* _resource, FILE* _file)
{
	bool bAssetExist = false;
	_resource == nullptr ? bAssetExist = false : bAssetExist = true;

	fwrite(&bAssetExist, sizeof(bool), 1, _file);

	if (bAssetExist)
	{
		std::wstring strKey = _resource->GetResourceName(); 
		size_t len = strKey.length(); 
		fwrite(&len, sizeof(size_t), 1, _file); 
		fwrite(strKey.c_str(), sizeof(wchar_t), strKey.length(), _file);

		std::wstring strRelativePath = _resource->GetResourcePath();
		len = strRelativePath.length(); 
		fwrite(&len, sizeof(size_t), 1, _file); 
		fwrite(strRelativePath.c_str(), sizeof(wchar_t), strRelativePath.length(), _file);
	}
}

#define SAVERESOURCEREF(_resource, _file) \
						bool bAssetExist = false;\
						_resource == nullptr ? bAssetExist = false : bAssetExist = true;\
						\
						fwrite(&bAssetExist, sizeof(bool), 1, _file);\
						\
						if (bAssetExist)\
						{\
							std::wstring strKey = _resource->GetResourceName();\
							size_t len = strKey.length();\
							fwrite(&len, sizeof(size_t), 1, _file);\
							fwrite(strKey.c_str(), sizeof(wchar_t), len, _file);\
						\
							std::wstring strRelativePath = _resource->GetResourcePath();\
							len = strRelativePath.length();\
							fwrite(&len, sizeof(size_t), 1, _file);\
							fwrite(strRelativePath.c_str(), sizeof(wchar_t), len, _file);\
						}\

template <typename T>
void LoadResourceRef(T*& _resource, FILE* _file)
{
	bool bAssetExist = false;
	fread(&bAssetExist, sizeof(bool), 1, _file);

	if (bAssetExist)
	{
		std::wstring strKey, strRelativePath;
		size_t len = 0; 
		wchar_t szBuff[256] = {}; 

		fread(&len, sizeof(size_t), 1, _file); 
		fread(szBuff, sizeof(wchar_t), len, _file); 
		strKey = szBuff; 

		wmemset(szBuff, 0, 256); 

		fread(&len, sizeof(size_t), 1, _file); 
		fread(szBuff, sizeof(wchar_t), len, _file); 
		strRelativePath = szBuff; 

		_resource = ResourceManager::GetInst()->Load<T>(strKey);
	}
}

#define LOADRESOURCEREF(_type, _resource, _file)\
			{\
				bool bAssetExist = false;\
				fread(&bAssetExist, sizeof(bool), 1, _file);\
				\
				if (bAssetExist)\
				{\
					std::wstring strKey, strRelativePath;\
					size_t len = 0;\
					wchar_t szBuff[256] = {};\
				\
					fread(&len, sizeof(size_t), 1, _file);\
					fread(szBuff, sizeof(wchar_t), len, _file);\
					strKey = szBuff;\
				\
					wmemset(szBuff, 0, 256);\
				\
					fread(&len, sizeof(size_t), 1, _file);\
					fread(szBuff, sizeof(wchar_t), len, _file);\
					strRelativePath = szBuff;\
				\
					_resource = ResourceManager::GetInst()->Load<_type>(strKey);\
				}\
			}



void SaveWString(const wstring& _str, FILE* _File);
void LoadWString(wstring& _str, FILE* _FILE);