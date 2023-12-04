#include "pch.h"
#include "Entity.h"

UINT Entity::m_NextID = 0;

Entity::Entity()
	: m_ID(m_NextID++)
	, m_Name()
	, mb_IsDead(false)
{
}

Entity::~Entity()
{
}

bool IsValid(Entity* _entity)
{
	if (_entity != nullptr && !_entity->GetIsDead())
		return true;
	else
		return false;
}