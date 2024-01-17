#pragma once
#include "define.h"
#include "Chapter.h"

enum class CHAPTER_LEVEL
{
	BASEMENT,
	CAVES,
	DEPTHS,
	WOMB,
	EDIT,
	END,
};

class ChapterManager
{
	SINGLETON(ChapterManager)
private:
	Chapter* m_Chapters[(UINT)CHAPTER_LEVEL::END];
	Chapter* m_CurChapter;

	bool m_bEditMode;

public:
	void Init();
	void Update();

	Chapter* GetCurChapter() { return m_CurChapter; }
	Chapter* GetChpater(CHAPTER_LEVEL _level) { return m_Chapters[(UINT)_level]; }

	void DetachGameObject(class GameObject* _obj);

	void RegisterObj(GameObject* _obj, LAYER_TYPE _layerType);

	void SetEditMode(bool _bValue);

private:
	void ChangeChapter(CHAPTER_LEVEL _level) { m_CurChapter = m_Chapters[(UINT)_level]; }

	friend class TaskManager;
};
