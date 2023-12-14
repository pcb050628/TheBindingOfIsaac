#include "pch.h"
#include "Test.h"

#include "AssetManager.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "TextureRenderer.h"

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
	for (GameObject* actr : m_Actors)
		actr->Update();
}

void Test::Render()
{
	for (GameObject* actr : m_Actors)
		actr->Render();
}
