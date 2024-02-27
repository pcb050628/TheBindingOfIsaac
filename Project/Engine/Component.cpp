#include "pch.h"
#include "Component.h"

Component::Component(COMPONENT_TYPE _type)
	: m_Owner(nullptr)
	, m_Type(_type)
{
}

Component::Component(const Component& _origin)
	: Entity(_origin)
	, m_Owner(nullptr)
	, m_Type(_origin.m_Type)
{
}

Component::~Component()
{
}
