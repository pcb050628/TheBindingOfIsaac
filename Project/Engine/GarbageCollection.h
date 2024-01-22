#pragma once

class GarbageCollection
{
	SINGLETON(GarbageCollection)
private:
	std::queue<class Entity*> m_Entities;

public:
	void Update();

	void Add(Entity* _entity) { m_Entities.push(_entity); }
};

