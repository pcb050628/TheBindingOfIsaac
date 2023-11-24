#include "pch.h"
#include "Engine.h"
#include "math.h"

#include "Time.h"
#include "Input.h"
#include "ChapterManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"

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

	Vec2 test1 = Vec2(10, 10);
	Vec2 test2 = Vec2(20 , 10);
	Vec2 test3 = test1 + test2;

	Time::GetInst()->Init();
	Input::GetInst()->Init();
	RenderManager::GetInst()->Init(m_hWnd);
	//ChapterManager::GetInst()->Init();
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
}

void Engine::Render()
{
	RenderManager::GetInst()->PrepareDraw();

	RenderManager::GetInst()->StartDraw();
	//ChapterManager::GetInst()->Render();
	RenderManager::GetInst()->EndDraw();
}

void Engine::SetWindowSize(const RECT& _Resolution, bool _menu)
{
	m_Resolution = _Resolution;
	AdjustWindowRect(&m_Resolution, WS_OVERLAPPEDWINDOW, _menu);
	SetWindowPos(m_hWnd, nullptr, 10, 10, m_Resolution.right - m_Resolution.left, m_Resolution.bottom - m_Resolution.top, 0);
}
