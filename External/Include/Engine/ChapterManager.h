#pragma once
#include "define.h"
#include "Chapter.h"

#include "AssetManager.h"

enum class CHAPTERLEVEL
{
	BASEMENT,
	CAVES,
	DEPTHS,
	WOMB,
	GMLEVEL,
	END,
};

class ChapterManager
{
	SINGLETON(ChapterManager)
private:
	Chapter* m_Chapters[(UINT)CHAPTERLEVEL::END];
	Chapter* m_CurChapter;

public:
	void Init();
	void Update();
	void Render();
	void LateUpdate();

	Chapter* GetCurChapter() { return m_CurChapter; }
	Chapter* GetChpater(CHAPTERLEVEL _level) { return m_Chapters[(UINT)_level]; }

	void DetachGameObject(class GameObject* _obj);

	void RegisterObj(GameObject* _obj, LAYER_TYPE _layerType);

private:
	void ChangeChapter(CHAPTERLEVEL _level) { m_CurChapter = m_Chapters[(UINT)_level]; }

	//void LoadChpaterFromAsset(AssetID _id);

	friend class TaskManager;
};
