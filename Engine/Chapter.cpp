#include "pch.h"
#include "Chapter.h"
#include "Room.h"

Chapter::Chapter()
	: m_Rooms()
	, m_CurRoom(nullptr)
{
}

Chapter::~Chapter()
{
}

void Chapter::Update()
{
	m_CurRoom->Update();
}

void Chapter::LateUpdate()
{
	m_CurRoom->LateUpdate();
}

void Chapter::Render()
{
	m_CurRoom->Render();
}
