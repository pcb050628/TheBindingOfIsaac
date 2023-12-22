#include "pch.h"
#include "PlayerController.h"

#include "Time.h"
#include "Input.h"

#include "GameObject.h"

PlayerController::PlayerController()
	: m_fSpeed(100.f)
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::Update()
{
	Vec3 vPos = GetOwner()->GetComponent<Transform>()->GetRelativePos();

	if (Input::GetInst()->CompareKeyState(Key::W, KeyState::Hold))
	{
		vPos.y += m_fSpeed * Time::GetInst()->GetDeltaTime();
	}

	if (Input::GetInst()->CompareKeyState(Key::A, KeyState::Hold))
	{
		vPos.x -= m_fSpeed * Time::GetInst()->GetDeltaTime();
	}

	if (Input::GetInst()->CompareKeyState(Key::S, KeyState::Hold))
	{
		vPos.y -= m_fSpeed * Time::GetInst()->GetDeltaTime();
	}

	if (Input::GetInst()->CompareKeyState(Key::D, KeyState::Hold))
	{
		vPos.x += m_fSpeed * Time::GetInst()->GetDeltaTime();
	}


	if (Input::GetInst()->CompareKeyState(Key::Up, KeyState::Hold))
	{
		Vec3 vScale = GetOwner()->GetComponent<Transform>()->GetRelativeScale();
		vScale.y += 10 * Time::GetInst()->GetDeltaTime();
		GetOwner()->GetComponent<Transform>()->SetRelativeScale(vScale);
	}

	if (Input::GetInst()->CompareKeyState(Key::Down, KeyState::Hold))
	{
		Vec3 vScale = GetOwner()->GetComponent<Transform>()->GetRelativeScale();
		vScale.y -= 10 * Time::GetInst()->GetDeltaTime();
		GetOwner()->GetComponent<Transform>()->SetRelativeScale(vScale);
	}

	GetOwner()->GetComponent<Transform>()->SetRelativePos(vPos);
}
