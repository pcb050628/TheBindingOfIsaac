#include "pch.h"
#include "HumanoidScript.h"


REGISTER_SCRIPT(HumanoidScript);
HumanoidScript::HumanoidScript() : Script(L"Humanoid")
	, Head(nullptr)
	, Body(nullptr)
	, HeadDir(DIRECTION::BottomDir)
	, BodyDir(DIRECTION::BottomDir)
{
}

HumanoidScript::~HumanoidScript()
{
}

void HumanoidScript::Init()
{
	if (Head != nullptr)
	{
		Head->DisconnectWithParent();
		delete Head;
		Head = nullptr;
	}
	if (Body != nullptr)
	{
		Body->DisconnectWithParent();
		delete Body;
		Body = nullptr;
	}

	Head = new GameObject(L"Head");
	Body = new GameObject(L"Body");

	Head->AddComponent(new Transform);
	Head->AddComponent(new Collider2D);

	Body->AddComponent(new Transform);
	Body->AddComponent(new Collider2D);

	GetOwner()->AttachChild(Head);
	GetOwner()->AttachChild(Body);

	//몸, 머리 위치 조정 해야함
}

void HumanoidScript::Update()
{
}