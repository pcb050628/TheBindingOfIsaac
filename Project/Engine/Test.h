#pragma once
#include "GameObject.h"

class Test
{
	SINGLETON(Test)
private:
	std::vector<GameObject*> m_Gobjs;

public:
	void Init();
	void Update();
	void LateUpdate();
	void Render();
};

