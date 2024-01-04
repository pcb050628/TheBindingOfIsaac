#pragma once

class Entity;
class GameObject;
enum class CHAPTERLEVEL;
enum DIRECTION;
enum class LAYER_TYPE;

bool IsValid(Entity* _entity);
void ChangeChapter(CHAPTERLEVEL _level);
void AddGameObject(GameObject* _actr, LAYER_TYPE _layer);
void ChangeRoom(DIRECTION _dir);
void Destroy(GameObject* _obj);
std::wstring GetContentPath();