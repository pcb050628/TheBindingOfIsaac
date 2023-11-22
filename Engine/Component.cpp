#include "pch.h"
#include "Component.h"

Component::Component(ComponentType _type)
	: m_Owner(nullptr)
	, m_Type(_type)
{
}

Component::~Component()
{
}

void Component::Update()
{

}

void Component::LateUpdate()
{
}
