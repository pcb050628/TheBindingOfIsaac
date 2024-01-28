#include "pch.h"
#include "SpotLight2DMove.h"


SpotLight2DMove::SpotLight2DMove()
{
}

SpotLight2DMove::~SpotLight2DMove()
{
}

void SpotLight2DMove::Update()
{
	Vec3 vRot = GetTransform()->GetRelativeRotation();
	Vec3 vPos = GetTransform()->GetRelativePos();

	if (KEY_HOLD(RIGHT))
	{
		vRot.z += 10.f * DT;
	}
	if (KEY_HOLD(LEFT))
	{
		vRot.z -= 10.f * DT;
	}

	if (KEY_HOLD(W))
	{
		vPos.y += 100.f * DT;
	}
	if (KEY_HOLD(A))
	{
		vPos.x -= 100.f * DT;
	}
	if (KEY_HOLD(S))
	{
		vPos.y -= 100.f * DT;
	}
	if (KEY_HOLD(D))
	{
		vPos.x += 100.f * DT;
	}


	GetTransform()->SetRelativeRotation(vRot);
	GetTransform()->SetRelativePos(vPos);
}