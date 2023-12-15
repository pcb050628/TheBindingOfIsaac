#include "pch.h"
#include "Test.h"

#include "AssetManager.h"
#include "ResourceManager.h"
#include "Texture.h"

Test::Test()
	: m_Actors()
{

}

Test::~Test()
{

}

void Test::Init()
{
}

void Test::Update()
{
	for (GameObject* gobj : m_Actors)
		gobj->Update();
}

void Test::Render()
{
	for (GameObject* gobj : m_Actors)
		gobj->Render();
}
