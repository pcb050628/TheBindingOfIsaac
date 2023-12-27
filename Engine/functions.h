#pragma once

class Entity;
class GameObject;
enum class CHAPTERLEVEL;
enum DIRECTION;

bool IsValid(Entity* _entity);
void ChangeChapter(CHAPTERLEVEL _level);
void AddGameObject(GameObject* _actr, CHAPTERLEVEL _level);
void ChangeRoom(DIRECTION _dir);
void Destroy(GameObject* _obj);
std::wstring GetContentPath();