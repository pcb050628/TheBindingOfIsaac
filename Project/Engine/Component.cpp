#include "pch.h"
#include "Component.h"

Component::Component(COMPONENT_TYPE _type)
	: m_Owner(nullptr)
	, m_Type(_type)
{
}

Component::~Component()
{
}
