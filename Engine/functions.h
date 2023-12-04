#pragma once

class Entity;
bool IsValid(Entity* _entity);
enum class CHAPTERLEVEL;
void ChangeChapter(CHAPTERLEVEL _level);
class Actor;
void AddActor(Actor* _actr, CHAPTERLEVEL _level);
enum DIRECTION;
void ChangeRoom(DIRECTION _dir);