#include "pch.h"
#include "Input.h"

int ASCII[(UINT)Key::END] =
{
	'Q','W','E','R','T','Y','U','I','O','P',
	'A','S','D','F','G','H','J','K','L',
	'Z','X','C','V','B','N','M',

	VK_UP, VK_LEFT, VK_DOWN, VK_RIGHT,
	VK_RBUTTON, VK_LBUTTON,
	VK_TAB,
};

Input::Input()
{

}

Input::~Input()
{

}

void Input::Init()
{
	for (int i = 0; i < (UINT)Key::END; i++)
	{
		KeyData data((Key)i);
		m_Keys.push_back(data);
	}
}

void Input::Update()
{
	for (int i = 0; i < (UINT)Key::END; i++)
	{
		if (GetKeyState(ASCII[i]) & 0x8000)
		{
			if (m_Keys[i].isPressed)
				m_Keys[i].state = KeyState::Hold;
			else
				m_Keys[i].state = KeyState::Pressed;

			m_Keys[i].isPressed = true;
		}
		else
		{
			if (m_Keys[i].isPressed)
				m_Keys[i].state = KeyState::Release;

			m_Keys[i].isPressed = false;
		}
	}
}

void Input::LateUpdate()
{
}
