#pragma once

class Entity;
class Actor;
enum class CHAPTERLEVEL;
enum DIRECTION;

bool IsValid(Entity* _entity);
void ChangeChapter(CHAPTERLEVEL _level);
void AddActor(Actor* _actr, CHAPTERLEVEL _level);
void ChangeRoom(DIRECTION _dir);