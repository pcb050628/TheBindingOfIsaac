#include "pch.h"
#include "TransformGUI.h"

#include "Engine/GameObject.h"
#include "Engine/Transform.h"

TransformGUI::TransformGUI() : GUI("TransformGUI", "##TransformGUI")
	, m_TargetObject(nullptr)
{
}

TransformGUI::~TransformGUI()
{
}

void TransformGUI::RenderUpdate()
{
	if (nullptr == m_TargetObject)
		return;


}

void TransformGUI::SetTargetObject(GameObject* _object)
{
}
