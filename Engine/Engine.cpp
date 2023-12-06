#include "pch.h"
#include "Engine.h"


#include "Time.h"
#include "Input.h"
#include "AssetManager.h"
#include "ChapterManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"

#include "Test.h"

Engine::Engine()
	: m_hWnd(nullptr)
	, m_DC(nullptr)
	, m_Resolution()
{
}

Engine::~Engine()
{
}

void Engine::Init(HWND _hWnd, const RECT& _Resolution)
{
	m_hWnd = _hWnd;
	SetWindowSize(_Resolution, false);
	ShowWindow(m_hWnd, true);

	m_DC = GetDC(m_hWnd);

	Vec2 res((float)m_Resolution.right, (float)m_Resolution.bottom);

	Time::GetInst()->Init();
	Input::GetInst()->Init();
	RenderManager::GetInst()->Init(m_hWnd, res);
	AssetManager::GetInst()->Init();
	//ChapterManager::GetInst()->Init();
	Test::GetInst()->Init();
}

void Engine::Run()
{
	Update();
	Render();
}

void Engine::Update()
{
	Time::GetInst()->Update();
	Input::GetInst()->Update();
	//ChapterManager::GetInst()->Update();
	//Test::GetInst()->Update();
}

void Engine::Render()
{
}

void Engine::SetWindowSize(const RECT& _Resolution, bool _menu)
{
	m_Resolution = _Resolution;
	AdjustWindowRect(&m_Resolution, WS_OVERLAPPEDWINDOW, _menu);
	SetWindowPos(m_hWnd, nullptr, 10, 10, m_Resolution.right - m_Resolution.left, m_Resolution.bottom - m_Resolution.top, 0);
}
