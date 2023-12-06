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
	Actor* a = new Actor();
	TextureRenderer* tr = a->AddComponent<TextureRenderer>();
	Texture* t = nullptr;
	//std::wstring path = GetContentPath();
	//path += L"Resource\\Image\\Rooms\\Basement.png";
	//AssetManager::GetInst()->Create<Texture>(AssetID::TEXTURE_CHAPTER1_BACKGROUND, path, L"basement_background_texture_resource");
	t = AssetManager::GetInst()->Load<Texture>(AssetID::TEXTURE_CHAPTER1_BACKGROUND);
	tr->SetTexture(t);
	m_Actors.push_back(a);
}

void Test::Update()
{
	for (Actor* actr : m_Actors)
		actr->Update();
}

void Test::Render()
{
	for (Actor* actr : m_Actors)
		actr->Render();
}
