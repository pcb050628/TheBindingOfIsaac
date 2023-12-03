#pragma once
#include "define.h"
#include "Chapter.h"

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

private:
	void ChangeChapter(CHAPTERLEVEL _level) { m_CurChapter = m_Chapters[(UINT)_level]; }

	friend class TaskManager;
};

void ChangeChapter(CHAPTERLEVEL _level);