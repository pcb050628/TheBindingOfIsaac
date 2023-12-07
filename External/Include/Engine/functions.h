#pragma once

class Entity;
class GameObject;
enum class CHAPTERLEVEL;
enum DIRECTION;

bool IsValid(Entity* _entity);
void ChangeChapter(CHAPTERLEVEL _level);
void AddActor(GameObject* _actr, CHAPTERLEVEL _level);
void ChangeRoom(DIRECTION _dir);
std::wstring GetContentPath();