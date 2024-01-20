#pragma once

class Entity;
class GameObject;
enum class CHAPTER_LEVEL;
enum DIRECTION;
enum class LAYER_TYPE;
enum class RESOURCE_TYPE;

bool IsValid(Entity* _entity);
void ChangeChapter(CHAPTER_LEVEL _level);
void AddGameObject(GameObject* _actr, LAYER_TYPE _layer);
void ChangeRoom(DIRECTION _dir);
void Destroy(GameObject* _obj);
std::wstring GetContentPath();
std::wstring GetResourceFolderPath(RESOURCE_TYPE _type);

std::wstring ToWstring(const std::string& _str);
std::string ToString(const std::wstring& _str);