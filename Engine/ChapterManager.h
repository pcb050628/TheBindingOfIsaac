#pragma once
#include "define.h"

using namespace std;

class Chapter;
class ChapterManager
{
	SINGLETON(ChapterManager)
private:
	vector<Chapter*> m_Chapters;
	Chapter* m_CurChapter;

public:
	void Init();
	void Update();
	void LateUpdate();

	Chapter* GetCurChapter() { return m_CurChapter; }
};

