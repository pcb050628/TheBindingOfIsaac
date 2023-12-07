#pragma once
#include "GameObject.h"

class Test
{
	SINGLETON(Test)
private:
	std::vector<GameObject*> m_Actors;

public:
	void Init();
	void Update();
	void Render();
};

