#pragma once
#include "Actor.h"

class Test
{
	SINGLETON(Test)
private:
	std::vector<Actor*> m_Actors;

public:
	void Init();
	void Update();
	void Render();
};

