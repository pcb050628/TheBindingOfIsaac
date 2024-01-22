#include "pch.h"
#include "GarbageCollection.h"

#include "Entity.h"

GarbageCollection::GarbageCollection()
{}

GarbageCollection::~GarbageCollection()
{}

void GarbageCollection::Update()
{
	if (m_Entities.size() > 5)
	{
		Entity* front = nullptr;
		while (!m_Entities.empty())
		{
			front = m_Entities.front();
			delete front;
			m_Entities.pop();
		}
	}
}
