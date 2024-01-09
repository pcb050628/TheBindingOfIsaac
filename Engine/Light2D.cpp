#include "pch.h"
#include "Light2D.h"

#include "RenderManager.h"

Light2D::Light2D() : Component(COMPONENT_TYPE::LIGHT2D)
	, m_Info()
{
}

Light2D::~Light2D()
{
}

void Light2D::LateUpdate()
{
}
