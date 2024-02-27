#include "pch.h"
#include "Entity.h"

UINT Entity::m_NextID = 0;

Entity::Entity()
	: m_ID(m_NextID++)
	, m_Name()
	, m_bIsDead(false)
{
}

Entity::Entity(const Entity& _origin)
	: m_ID(m_NextID++)
	, m_Name(_origin.m_Name)
	, m_bIsDead(false)
{

}

Entity::~Entity()
{
}
