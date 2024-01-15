#pragma once
#include <windows.h>
#include "define.h"

class Engine
{
private:
	SINGLETON(Engine)

private:
	HWND	m_hWnd;
	HDC		m_DC;

	RECT	m_Resolution;

public:
	void Init(HWND _hWnd, const RECT& _Resolution);
	void Progress();

	void SetWindowSize(const RECT& _Resolution, bool _menu);

	HWND GetHwnd() { return m_hWnd; }

	RECT GetResolution() { return m_Resolution; }
};

