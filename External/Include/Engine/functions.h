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

void DrawDebugRect(const Matrix& _WorldMat, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);
void DrawDebugRect(Vec3 _vWorldPos, Vec3 _vWorldScale, Vec3 _vWorldRot, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

void DrawDebugCircle(Vec3 _vWorldPos, float _fRadius, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

void DrawDebugCross(Vec3 _vWorldPos, float _fScale, Vec3 _Color, bool _bDepthTest, float _Duration = 0.f);

std::wstring ToWstring(const std::string& _str);
std::string ToString(const std::wstring& _str);